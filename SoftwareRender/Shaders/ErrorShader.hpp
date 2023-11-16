#ifndef __ERRORSHADER__H__
#define __ERRORSHADER__H__

#include "../ShaderLibrary/IShader.hpp"
#include "../ShaderLibrary/ShaderCore.hpp"

class ErrorShader : public IShader {
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
	ErrorShader() {
		bindData(&attributes, &varyings);
		attri_semantic = (int)SEMANTIC::vertex;
		addPass(PROGRAM(ErrorShader, vertex), PROGRAM(ErrorShader, fragment));
	}

	vec4 vertex() {
		vec4 vertex = attributes.vertex;

		varyings.posCS = uniform_ObjectToClipH * vertex;

		return varyings.posCS;
	}

	vec4 fragment() {
		return { 0.854902,0.439216, 0.839216, 1 };
	}
};

#endif