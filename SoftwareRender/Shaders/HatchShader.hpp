#ifndef __HATCHSHADER__H__
#define __HATCHSHADER__H__

#include "../ShaderLibrary/IShader.hpp"
#include "../ShaderLibrary/ShaderCore.hpp"

class HatchShader : public IShader {
public:
	struct Properties {
		Texture hatch0;
		Texture hatch1;
		Texture hatch2;
		Texture hatch3;
		Texture hatch4;
		Texture hatch5;

		vec4 baseColor = { 255, 255, 255, 255 };
		vec4 outlineColor = { 0, 0, 0, 255 };

		double outline = 0.015;
		double tileFactor = 1.0;
	}properties;

private:
	struct Attributes {
		vec4 vertex;
		vec4 normal;
		vec4 texcoord;
	}attributes;

	struct Varyings {
		vec4 posCS;
		vec4 posWS;
		vec4 uv;
		vec4 hatchWeights0;
		vec4 hatchWeights1;
	}varyings;

public:
	HatchShader() {
		bindData(&attributes, &varyings);
		attri_semantic = (int)SEMANTIC::vertex | (int)SEMANTIC::normal | (int)SEMANTIC::texcoord;

		// pass0
		passSettings outlinePassSettings = passSettings();
		outlinePassSettings.cull = 1;
		addPass(PROGRAM(HatchShader, outlineVertex), PROGRAM(HatchShader, outlineFragment), outlinePassSettings);

		// pass1
		passSettings shadingPassSettings = passSettings();
		shadingPassSettings.cull = 0;
		addPass(PROGRAM(HatchShader, shadingVertex), PROGRAM(HatchShader, shadingFragment), shadingPassSettings);
	}

	// Outline
	vec4 outlineVertex() {
		vec4 pos = attributes.vertex;

		vec3 normal = attributes.normal;
		normal.z = -0.5;
		pos = pos + vec4(normal.normalized()) * properties.outline;

		return 	varyings.posCS = uniform_ObjectToClipH * pos;;
	}

	vec4 outlineFragment() {
		vec3 finalColor = properties.outlineColor / 255.0;
		return finalColor.Appended<1>({ 1 });
	}

	// Shading
	vec4 shadingVertex() {
		varyings.posWS = uniform_ObjcetToWorld * attributes.vertex;
		varyings.uv = attributes.texcoord * properties.tileFactor;

		vec3 lDirWS = uniform_lightDirection.normalized();
		vec3 nDirWS = (uniform_ObjcetToWorld * attributes.normal).normalized();

		double diffuse = saturate(uniform_lightIntensity * dot(lDirWS, nDirWS));

		varyings.hatchWeights0 = 0.0;
		varyings.hatchWeights1 = 0.0;

		double hatchFactor = diffuse * 7.0;

		if (hatchFactor > 6.0) {

		}
		else if (hatchFactor > 5.0) {
			varyings.hatchWeights0.x = hatchFactor - 5.0;
		}
		else if (hatchFactor > 4.0) {
			varyings.hatchWeights0.x = hatchFactor - 4.0;
			varyings.hatchWeights0.y = 1.0 - varyings.hatchWeights0.x;
		}
		else if (hatchFactor > 3.0) {
			varyings.hatchWeights0.y = hatchFactor - 3.0;
			varyings.hatchWeights0.z = 1.0 - varyings.hatchWeights0.y;
		}
		else if (hatchFactor > 2.0) {
			varyings.hatchWeights0.z = hatchFactor - 2.0;
			varyings.hatchWeights1.x = 1.0 - varyings.hatchWeights0.z;
		}
		else if (hatchFactor > 1.0) {
			varyings.hatchWeights1.x = hatchFactor - 1.0;
			varyings.hatchWeights1.y = 1.0 - varyings.hatchWeights1.x;
		}
		else {
			varyings.hatchWeights1.y = hatchFactor;
			varyings.hatchWeights1.z = 1.0 - varyings.hatchWeights1.y;
		}

		return varyings.posCS = uniform_ObjectToClipH * attributes.vertex;
	}

	vec4 shadingFragment() {
		vec4 hatchTex0 = tex2D(properties.hatch0, varyings.uv) * varyings.hatchWeights0.x;
		vec4 hatchTex1 = tex2D(properties.hatch1, varyings.uv) * varyings.hatchWeights0.y;
		vec4 hatchTex2 = tex2D(properties.hatch2, varyings.uv) * varyings.hatchWeights0.z;
		vec4 hatchTex3 = tex2D(properties.hatch3, varyings.uv) * varyings.hatchWeights1.x;
		vec4 hatchTex4 = tex2D(properties.hatch4, varyings.uv) * varyings.hatchWeights1.y;
		vec4 hatchTex5 = tex2D(properties.hatch5, varyings.uv) * varyings.hatchWeights1.z;
		vec4 whiteColor = vec4{ 1, 1, 1, 1 } *  ( 1 - varyings.hatchWeights0.x - varyings.hatchWeights0.y - varyings.hatchWeights0.z -
			varyings.hatchWeights1.x - varyings.hatchWeights1.y - varyings.hatchWeights1.z );

		vec4 hatchColor = hatchTex0 + hatchTex1 + hatchTex2 + hatchTex3 + hatchTex4 + hatchTex5 + whiteColor;

		auto baseColor = properties.baseColor / 255.0;

		vec3 finalColor = hatchColor.xyz() * baseColor.xyz();

		return finalColor.Appended<1>({ 1 });
	}
};

#endif