#ifndef __RAMPSHADER__H__
#define __RAMPSHADER__H__

#include "../ShaderLibrary/IShader.hpp"
#include "../ShaderLibrary/ShaderCore.hpp"

class RampShader : public IShader {
public:
	struct Properties {
		Texture rampTex;
		double outline = 0.015;
		vec4 outlineColor = { 0, 0, 0, 255 };
	} properties;

private:
	struct A2v {
		vec4 vertex;
		vec4 normal;
	} a2v;

	struct V2f {
		vec4 posCS; // (x, y) screenUV
		vec4 nDirWS;
	} v2f;

public:
	RampShader() {
		bindData(&a2v, &v2f);
		attri_semantic = (int)SEMANTIC::vertex | (int)SEMANTIC::normal;

		passSettings outlinePassSettings = passSettings();
		outlinePassSettings.cull = 1;
		addPass(PROGRAM(RampShader, outlineVertex), PROGRAM(RampShader, outlineFragment), outlinePassSettings);

		passSettings shadingPassSettings = passSettings();
		shadingPassSettings.cull = 0;
		addPass(PROGRAM(RampShader, vertex), PROGRAM(RampShader, fragment), shadingPassSettings);
	}

	// Outline
	vec4 outlineVertex() {
		vec4 pos = a2v.vertex;

		vec3 normal = a2v.normal;
		normal.z = -0.5;
		pos = pos + vec4(normal.normalized()) * properties.outline;

		return 	v2f.posCS = uniform_ObjectToClipH * pos;;
	}

	vec4 outlineFragment() {
		vec3 finalColor = properties.outlineColor / 255.0;
		return finalColor.Appended<1>({ 1 });
	}

	// Shading
	vec4 vertex() {
		vec4 vertex = a2v.vertex;

		v2f.posCS = uniform_ObjectToClipH * vertex;
		v2f.nDirWS = uniform_ObjcetToWorld * a2v.normal;

		return v2f.posCS;
	}

	vec4 fragment() {
		vec3 nDir = v2f.nDirWS.normalized();
		vec3 lDir = (uniform_lightDirection).normalized();

		double halfLambert = dot(nDir, lDir) * 0.5 + 0.5;

		vec4 var_ramp = tex2D(properties.rampTex, halfLambert);

		vec3 finalColor = var_ramp.xyz();

		return finalColor.Appended<1>({ 1.0 });
	}
};

#endif