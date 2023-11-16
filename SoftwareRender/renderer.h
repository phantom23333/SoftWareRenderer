#ifndef __RENDERER__H__
#define __RENDERER__H__

#include <QApplication>
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include "framebuffer.h"
#include "model.h"
#include "camera.h"
#include "Common/geometry.h"
#include "Common/texture.hpp"
#include "light.h"
#include "gameTimer.h"
#include "ShaderLibrary/IShader.hpp"
#include "Shaders/PhongShader.hpp"
#include "Shaders/RampShader.hpp"

class Renderer {
public:
	int mHeight, mWidth;
private:
	// 计算重心坐标系系数(1-u-v, u, v) P = (1-u-v)A+uB+vC
	vec3 barycentric(vec4* pts, vec3 p);

	double interpolate(const vec3& val, const vec3& weight);

public:
	Renderer() = default;

	Renderer(int width, int height) : mWidth(width), mHeight(height) {}

public:
	// 渲染
	template <typename T>
	void render(Model* model, FrameBuffer& buffer, T* shader, const int& pass, const mat<4, 4>& viewport, double* zbuffer, bool renderColorBuffer = true, bool renderDepthBuffer = false) {
		// shader必须是IShader的子列
		static_assert(std::is_base_of<IShader, T>::value, "T must be a subclass of IShader");

		auto colorBuffer = buffer.colorBuffer();

		if (model == nullptr) return;

		// 顶点着色器后的屏幕映射坐标集合
		std::vector<vec4> posSS;
		// Shader用于插值的Varyings数据集合
		std::vector<std::vector<vec4>> varyingss;

		shaderPass thePass = shader->pass(pass);
		auto vertexProgram = thePass.vertex;
		auto fragmentProgram = thePass.fragment;
		auto settings = thePass.settings;

		int cullMode = model->settings.override ? model->settings.cull : settings.cull;
		bool zTest = model->settings.override ? model->settings.zTest : settings.zTest;
		bool zWrite = model->settings.override ? model->settings.zWrite : settings.zWrite;

		// 几何阶段
		// vertex shader
		for (int i = 0; i < model->nfaces(); i++) {
			// 三角形是否被剔除
			bool culledByProjection = true;
			// 三角行的屏幕映射坐标和用于插值的数据
			vec4 tmp_posSS[3];
			std::vector<vec4> tmp_varyingss[3];
			for (int j = 0; j < 3; j++) {
				// 传递着色器attributes
				auto semantic = shader->getAttriSemantic();
				// 要求必须输入顶点语义
				assert(semantic & (int)SEMANTIC::vertex);
				int attr_i = 0; // attributes下标计数
				// vertex
				auto vertex = model->modelMat() * model->vert(i, j).Appended<1>({ 1 });
				shader->setAttributes(attr_i++, vertex);
				// normal
				auto normal = model->normal(i, j);
				if (semantic & (int)SEMANTIC::normal) {
					shader->setAttributes(attr_i++, normal);
				}
				// tangent
				auto tangent = model->tangent(i);
				if (semantic & (int)SEMANTIC::tangent) {
					shader->setAttributes(attr_i++, tangent.Appended<1>({ 1 }));
				}

				// texcoord
				auto texcoord = model->uv(i, j);
				if (semantic & (int)SEMANTIC::texcoord) {
					shader->setAttributes(attr_i++, texcoord);
				}

				// TODO: model类没有加顶点色

				// 顶点着色器阶段
				auto posCS = vertexProgram();

				double& w = posCS[3];
				// 判断这个顶点是否会被透视投影剔除
				if (posCS[0] >= -w && posCS[0] <= w && posCS[1] >= -w && posCS[1] <= w && posCS[2] >= -w && posCS[2] <= w)
					culledByProjection = false;
				// 进行齐次除法
				if (w < 1e-7) w = 1e-7;
				for (int i = 0; i < 3; i++) posCS[i] /= w;
				w = 1;

				// 屏幕映射
				tmp_posSS[j] = discretize(viewport * posCS);
				// 存储顶点着色器处理后的用于插值的值
				tmp_varyingss[j] = shader->getVaryings();
			}
			// 如果整个三角形都没被透视投影剔除 则存储数据
			if (!culledByProjection) {
				// 背面剔除阶段
				// 根据面积符号判断朝向
				vec2 e1 = tmp_posSS[1] - tmp_posSS[0], e2 = tmp_posSS[2] - tmp_posSS[1];
				auto area = cross(e1, e2);
				if (cullMode == 0 && area.x < 0) continue;// cull back
				if (cullMode == 1 && area.x > 0) continue;// cull front
				for (int j = 0; j < 3; j++) {
					posSS.push_back(tmp_posSS[j]);
					varyingss.push_back(tmp_varyingss[j]);
				}
			}
		}

		// 光栅化阶段
		// 三角形遍历
		for (int tri = 0; tri < posSS.size(); tri += 3) {
			// 三角形选择
			vec4 pts[3] = { posSS[tri], posSS[tri + 1], posSS[tri + 2] };
			std::vector<vec4> varyings[3] = { varyingss[tri], varyingss[tri + 1], varyingss[tri + 2] };

			// 计算AABB
			vec2 bboxmin = { mWidth - 1.0, mHeight - 1.0 };
			vec2 bboxmax = { -1, -1 };
			vec2 clamp = { mWidth - 1.0, mHeight - 1.0 };
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 2; j++) {
					bboxmin[j] = std::max(0.0, std::min(bboxmin[j], pts[i][j]));
					bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
				}
			}

			// 遍历像素
			vec3 p;
			for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++) {
				for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++) {
					// 计算重心坐标系数
					vec3 weight = barycentric(pts, p);
					// 舍弃不在三角形内的像素
					if (weight.x < 0 || weight.y < 0 || weight.z < 0) continue;

					// zTest阶段
					p.z = interpolate({ pts[0].z, pts[1].z, pts[2].z }, weight);
					// 由于相机右手坐标系 -z为forward 所以zbuffer为负 越大的越前面
					if (zTest && zbuffer[int(p.x + p.y * mWidth)] < p.z) continue;
					// zWrite阶段
					if (zWrite) {
						zbuffer[int(p.x + p.y * mWidth)] = p.z;
						if (renderDepthBuffer)
							buffer.depthBuffer().set(p.x, p.y, saturate(p.z));
					}

					// 重心坐标插值 将Shader端的Varyings全部进行插值
					for (int i = 0; i < shader->getVaryingsLen(); i++) {
						vec4 varying;
						for (int j = 0; j < 4; j++)
							varying[j] = interpolate({ varyings[0][i][j], varyings[1][i][j], varyings[2][i][j] }, weight);

						shader->setVaryings(i, varying);
					}

					// fragment shader
					vec4 fragment = fragmentProgram();

					// clipped
					if (fragment.x == -233 && fragment.y == -233 && fragment.z == -233 && fragment.w == -233) continue;

					// 计算最终颜色
					fragment = saturate(fragment);

					if(renderColorBuffer)
						colorBuffer.set(p.x, p.y, fragment.xyz());
				}
			}
		}
	}
};

#endif