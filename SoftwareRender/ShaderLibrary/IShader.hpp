#ifndef __ISHADER_H__
#define __ISHADER_H__

#include "../Common/geometry.h"
#include "../Common/texture.hpp"
#include "../framebuffer.h"
#include <functional>
#include <string>
#include <vector>

#define PROGRAM(className, program)\
	std::bind(&className::program, this)

// 定义一个pass由顶点着色器、片元着色器函数和pass设置组成
struct passSettings {
	bool zTest = true;
	bool zWrite = true;
	int cull = 0;// 0 cull back 1 cull froint 2 cull none
};

struct shaderPass {
	shaderPass(std::function<vec4(void)> vertex, std::function<vec4(void)> fragment, passSettings settings) : vertex(vertex), fragment(fragment), settings(settings) {}
	std::function<vec4(void)> vertex;
	std::function<vec4(void)> fragment;
	passSettings settings;
};

// 顶点着色器输入语义
enum class SEMANTIC {
	vertex = 1 << 0,
	normal = 1 << 1,
	tangent = 1 << 2,
	texcoord = 1 << 3,
	color = 1 << 4
};

class IShader {
private:
	std::vector<vec4*> mAttributes, mVaryings;
	std::vector <std::pair<std::string, double*>> mDoubleProperties;
	std::vector<std::pair<std::string, vec4*>> mVecProperties;
	std::vector<std::pair<std::string, Texture*>> mTexProperties;
	std::vector<shaderPass> mPasses;

protected:
	int attri_semantic;

public:
	mat<4, 4> uniform_ObjectToClipH;
	mat<4, 4> uniform_ObjcetToWorld;
	mat<4, 4> uniform_lightMVP;
	vec4 uniform_lightColor;
	vec4 uniform_lightDirection;
	vec4 uniform_ambient;
	vec4 uniform_worldSpaceCameraPos;
	vec4 uniform_time;
	double uniform_lightIntensity;

	DepthBuffer uniform_shadowmap;

private:
	template <typename T>
	std::vector<vec4*> struct2vector(T* st) {
		std::vector<vec4*> res;
		void* it = st;
		for (int i = 0; i < sizeof(*st) / sizeof(vec4); i++) {
			vec4* elem = (vec4*)it;
			res.push_back(elem);
			it = (void*)((char*)it + sizeof(vec4));
		}
		return res;
	}

protected:
	// 绑定attibutes和varyings的结构体与指针vector
	template <typename T1, typename T2>
	void bindData(T1* Attributes, T2* Varyings) {
		mAttributes = struct2vector(Attributes);
		mVaryings = struct2vector(Varyings);
	}

	// 添加pass
	void addPass(const std::function<vec4(void)>& vertex, const std::function<vec4(void)>& fragment, passSettings settings = passSettings()) {
		mPasses.push_back(shaderPass(vertex, fragment, settings));
	}



public:
	IShader() = default;

	virtual ~IShader() {
		for (auto t : mAttributes) {
			t = nullptr;
		}
		for (auto t : mVaryings) {
			t = nullptr;
		}
	}

	// 获得pass
	shaderPass pass(const int& i) {
		assert(i >= 0 && i < mPasses.size());
		return mPasses[i];
	}


	int passNum() const {
		return mPasses.size();
	}

	int getAttriSemantic() {
		return (int)attri_semantic;
	}

	std::vector<vec4> getAttributes() {
		std::vector<vec4> res;
		for (auto t : mAttributes) res.push_back(*t);
		return res;
	}

	std::size_t getAttributesLen() {
		return mAttributes.size();
	}

	void setAttributes(int i, vec4 attr) {
		*(mAttributes[i]) = attr;
	}

	std::vector<vec4> getVaryings() {
		std::vector<vec4> res;
		for (auto t : mVaryings) res.push_back(*t);
		return res;
	}

	void setVaryings(int i, vec4 vary) {
		*(mVaryings[i]) = vary;
	}

	std::size_t getVaryingsLen() {
		return mVaryings.size();
	}

	void addDoubleProperties(std::string name, double* address) {
		mDoubleProperties.push_back(std::make_pair(name, address));
	}

	auto getDoubleProperties() {
		return mDoubleProperties;
	}

	void addVecProperties(std::string name, vec4* address) {
		mVecProperties.push_back(std::make_pair(name, address));
	}

	auto getVecProperties() {
		return mVecProperties;
	}

	void addTexProperties(std::string name, Texture* address) {
		mTexProperties.push_back(std::make_pair(name, address));
	}

	auto getTexProperties() {
		return mTexProperties;
	}
};
#endif
