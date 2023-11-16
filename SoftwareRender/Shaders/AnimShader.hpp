#ifndef __ANIMSHADER__H__
#define __ANIMSHADER__H__

#include "../ShaderLibrary/IShader.hpp"
#include "../ShaderLibrary/ShaderCore.hpp"

class AnimShader : public IShader {
public:
	struct Properties {
		Texture noiseMap;
		vec4 effParams = { 1.5, 30.0, 0.2, 0.1 };
		vec4 baseColor = { 255, 255, 255, 0 };
	} properties;

private:
	struct Atrributes {
		vec4 vertex;
		vec4 normal;
		vec4 texcoord;
	} attributes;

	struct Varyings {
		vec4 posCS; // (x, y) screenUV
		vec4 mask;
		vec4 nDirWS;
	} varyings;

public:
	AnimShader() {
		bindData(&attributes, &varyings);
		attri_semantic = (int)SEMANTIC::vertex | (int)SEMANTIC::normal | (int)SEMANTIC::texcoord;
		addPass(PROGRAM(AnimShader, vertex), PROGRAM(AnimShader, fragment));
	}

	vec4 vertex() {
		vec4 vertex = attributes.vertex;

		float baseMask = abs(frac(vertex.y * properties.effParams.x - uniform_time.x * properties.effParams.y) - 0.5) * 2;
		baseMask = min(1.0, baseMask * 2.0);
		float noise = tex2D(properties.noiseMap, attributes.texcoord).x;
		baseMask += (1 - baseMask) * (noise - 0.5) * 5;

		varyings.mask = baseMask;

		vertex.x += attributes.normal.x * (1.0 - baseMask) * properties.effParams.w * baseMask;
		vertex.z += attributes.normal.z * (1.0 - baseMask) * properties.effParams.w * baseMask;

		varyings.nDirWS = uniform_ObjcetToWorld * attributes.normal;

		return varyings.posCS = uniform_ObjectToClipH * vertex;
	}

	vec4 fragment() {
		double opactiy = varyings.mask.x;

		clip(opactiy - properties.effParams.z);

		vec3 baseColor = properties.baseColor / 255.0;
		vec3 randColor = { std::sinf(uniform_time.x * 20.0) * 0.5 + 0.5 , std::cosf(uniform_time.x * 30.0 + 3) * 0.5 + 0.5, std::sinf(uniform_time.x * 90.0 - 10) * 0.5 + 0.5 };

		vec3 finalColor = baseColor * randColor;

		return finalColor;
	}
};

#endif