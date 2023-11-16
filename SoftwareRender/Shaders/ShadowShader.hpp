#ifndef __SHADOWSHADER__H__
#define __SHADOWSHADER__H__

#include "../ShaderLibrary/IShader.hpp"
#include "../ShaderLibrary/ShaderCore.hpp"

class ShadowShader : public IShader {
public:
	struct Properties {

	} properties;

private:
	struct Atrributes {
		vec4 vertex;
	} attributes;

	struct Varyings {
		vec4 posCS; // (x, y) screenUV
	} varyings;

public:
	ShadowShader() {
		bindData(&attributes, &varyings);
		attri_semantic = (int)SEMANTIC::vertex;
		addPass(PROGRAM(ShadowShader, vertex), PROGRAM(ShadowShader, fragment));
	}

	vec4 vertex() {
		vec4 vertex = attributes.vertex;

		varyings.posCS = uniform_ObjectToClipH * vertex;

		return varyings.posCS;
	}

	vec4 fragment() {
		return { 1.0, 1.0, 1.0, 1.0 };
	}
};

#endif