#ifndef __RENDERPIPELINE__H__
#define __RENDERPIPELINE__H__

#include <QApplication>
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include "renderer.h"
#include "framebuffer.h"
#include "model.h"
#include "camera.h"
#include "Common/geometry.h"
#include "Common/texture.hpp"
#include "light.h"
#include "gameTimer.h"
#include "ShaderLibrary/IShader.hpp"
#include "Shaders/ShadowShader.hpp"

class RenderPipeline {
private:
	Renderer mRenderer;
	FrameBuffer mFrameBuffer;

	Camera mCamera;
	Light mLight;
	GameTimer mGameTimer;

	int mWidth, mHeight;
	int mFps;

private:
	mat<4, 4> calcLightMVP(const vec3& translate, const vec3& scale);

public:
	RenderPipeline() = default;
	RenderPipeline(int width, int height);

	FrameBuffer& framebuffer() { return mFrameBuffer; }
	Camera& camera() { return mCamera; }
	Light& light() { return mLight; }

	void setLightColor(double r, double g, double b);
	void setLightIntensity(double intensity);
	void setLightDirection(vec3 direction);
	void setAmbient(double r, double g, double b);

public:
	// 渲染管线
	template<typename T>
	double render(std::vector<Model*> models, T* shader, const int shaderIdx, int& fenceGPU) {
		mGameTimer.Tick();

		// 更新相机
		mCamera.updated();

		// 绘制shadowmap
		mFrameBuffer.depthBuffer().clear();

		double* zbuffer = new double[mWidth * mHeight];
		for (int i = 0; i < mWidth * mHeight; i++) zbuffer[i] = 100;

		//ShadowShader* shadowShader = new ShadowShader();

		//for (auto model : models) {
		//	shadowShader->uniform_ObjectToClipH = calcLightMVP(model->position(), model->scaling());
		//	//shadowShader->uniform_ObjectToClipH = mCamera.projection() * mCamera.view();
		//	mRenderer.render(model, mFrameBuffer, shadowShader, 0, mCamera.viewport(), zbuffer, false, true);
		//}

		// 绘制所有模型

		// 传shader全局属性
		shader->uniform_ObjectToClipH = mCamera.projection() * mCamera.view();
		shader->uniform_ObjcetToWorld = mCamera.view();
		shader->uniform_lightColor = mLight.color * mLight.intensity;
		shader->uniform_lightDirection = mLight.direction.normalized();
		shader->uniform_ambient = mLight.ambient;
		shader->uniform_worldSpaceCameraPos = mCamera.worldPos().Appended<1>({ 1 });
		shader->uniform_time = { mGameTimer.runTime() / 20.0, mGameTimer.runTime() * 2.0, mGameTimer.runTime() * 3.0, 1 };
		shader->uniform_shadowmap = mFrameBuffer.depthBuffer();
		shader->uniform_lightIntensity = mLight.intensity;

		// clear framebuffer
		//mFrameBuffer.colorBuffer().clear({ 0.462745,0.843137,0.917647 });
		mFrameBuffer.colorBuffer().clear({ 0.171875,0.242188,0.3125 });

		// set Zbuffer
		for (int i = 0; i < mWidth * mHeight; i++) zbuffer[i] = 100;

		for (auto model : models) {
			// 传shader properties
			if (shaderIdx != -1) {
				int pid = 0;
				auto doubleProperties = shader->getDoubleProperties();
				for (auto t : model->doubleProperties[shaderIdx]) {
					double* address = doubleProperties[pid++].second;
					*address = *t;
				}

				pid = 0;
				auto vecProperties = shader->getVecProperties();
				for (auto t : model->vecProperties[shaderIdx]) {
					vec4* address = vecProperties[pid++].second;
					*address = *t;
				}

				pid = 0;
				auto texProperties = shader->getTexProperties();
				for (auto t : model->texProperties[shaderIdx]) {
					Texture* address = texProperties[pid++].second;
					*address = *t;
				}
			}

			shader->uniform_lightMVP = calcLightMVP(model->position(), model->scaling());

			for (int pass = 0; pass < shader->passNum(); pass++) {
				mRenderer.render(model, mFrameBuffer, shader, pass, mCamera.viewport(), zbuffer);
			}
		}

		return mGameTimer.GetFps();
	}
};

#endif