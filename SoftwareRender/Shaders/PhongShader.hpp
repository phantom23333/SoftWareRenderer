#ifndef __PHONGSHADER__H__
#define __PHONGSHADER__H__

#include "../ShaderLibrary/IShader.hpp"
#include "../ShaderLibrary/ShaderCore.hpp"

class PhongShader : public IShader {
public:
	struct Properties {
		Texture mainTex;
		Texture normTex = Texture(true);// normalTex
		Texture specTex = Texture(255, 255, 255);
		double gloss = 5500.0;
		double bumpScale = 1.0;
		vec4 baseColor = { 255, 255, 255, 0 };
		vec4 specColor = { 255, 255, 255, 0 };
	} properties;

private:
	struct Atrributes {
		vec4 vertex;
		vec4 normal;
		vec4 tangent;
		vec4 texcoord;
	} attributes;

	struct Varyings {
		vec4 posCS; // (x, y) screenUV
		vec4 posWS;
		vec4 nDirWS;
		vec4 tDirWS;
		vec4 bDirWS;
		vec4 uv;
	} varyings;

public:
	PhongShader() {
		bindData(&attributes, &varyings);
		attri_semantic = (int)SEMANTIC::vertex | (int)SEMANTIC::normal | (int)SEMANTIC::texcoord | (int)SEMANTIC::tangent;
		addPass(PROGRAM(PhongShader, vertex), PROGRAM(PhongShader, fragment));
	}

	vec4 vertex() {
		vec4 vertex = attributes.vertex;

		varyings.posCS = uniform_ObjectToClipH * vertex;
		varyings.posWS = uniform_ObjcetToWorld * vertex;
		varyings.nDirWS = uniform_ObjcetToWorld * attributes.normal;
		varyings.tDirWS = uniform_ObjcetToWorld * attributes.tangent;
		varyings.bDirWS = cross(varyings.nDirWS.xyz(), varyings.tDirWS.xyz());
		varyings.uv = attributes.texcoord;

		return varyings.posCS;
	}

	vec4 fragment() {
		vec3 bump = unpackNormal(tex2D(properties.normTex, varyings.uv), properties.bumpScale);
		mat<3, 3> TBN;
		TBN.set_col(0, varyings.tDirWS);
		TBN.set_col(1, varyings.bDirWS);
		TBN.set_col(2, varyings.nDirWS);

		vec3 nDir = (TBN * bump).normalized();
		vec3 lDir = (uniform_lightDirection).normalized();
		vec3 vDir = (uniform_worldSpaceCameraPos.xyz() - varyings.posWS.xyz()).normalized();
		vec3 hDir = (lDir + vDir).normalized();

		vec3 albedo = tex2D(properties.mainTex, varyings.uv).xyz();

		vec3 baseColor = properties.baseColor.xyz() / 255.0;
		vec3 specColor = properties.specColor.xyz() / 255.0;

		double gloss = tex2D(properties.specTex, varyings.uv).x * properties.gloss;

		vec3 ambient = uniform_ambient.xyz() * albedo;
		vec3 diffuse = uniform_lightColor.xyz() * albedo * saturate(dot(nDir, lDir));
		vec3 specular = specColor * uniform_lightColor.xyz() * std::pow(saturate(dot(nDir, hDir)), gloss);

		vec4 finalColor = baseColor * (ambient + diffuse + specular);

		return finalColor.xyz().Appended<1>({ 1.0 });
	}
};

#endif