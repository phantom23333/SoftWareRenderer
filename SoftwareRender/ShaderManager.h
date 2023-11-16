#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <vector>
#include <string>
#include "Shaders/PhongShader.hpp"
#include "Shaders/RampShader.hpp"
#include "Shaders/AnimShader.hpp"
#include "Shaders/ErrorShader.hpp"
#include "Shaders/ShadowShader.hpp"
#include "Shaders/HatchShader.hpp"

namespace SHADERMANAGER {
	const int number = 4;
	const std::string names[number] = { "PhongShader", "RampShader", "AnimShader" , "HatchShader"};
}

struct ShaderManager {
	ErrorShader errorShader;
	PhongShader phongShader;
	RampShader rampShader;
	AnimShader animShader;
	ShadowShader shadowShader;
	HatchShader hatchShader;

	void initShader() {
		// 初始化PhongShader
		phongShader.addDoubleProperties("Gloss", &phongShader.properties.gloss);
		phongShader.addDoubleProperties("BumpScale", &phongShader.properties.bumpScale);
		phongShader.addVecProperties("BaseColor", &phongShader.properties.baseColor);
		phongShader.addVecProperties("SpecColor", &phongShader.properties.specColor);
		phongShader.addTexProperties("MainTex", &phongShader.properties.mainTex);
		phongShader.addTexProperties("NormTex", &phongShader.properties.normTex);
		phongShader.addTexProperties("SpecTex", &phongShader.properties.specTex);

		// 初始化RampShader
		rampShader.addTexProperties("RampTex", &rampShader.properties.rampTex);
		rampShader.addDoubleProperties("Outline", &rampShader.properties.outline);
		rampShader.addVecProperties("Outline Color", &rampShader.properties.outlineColor);

		// 初始化AnimShader
		animShader.addTexProperties("NoiseMap", &animShader.properties.noiseMap);
		animShader.addVecProperties("BaseColor", &animShader.properties.baseColor);
		animShader.addVecProperties("EffParm", &animShader.properties.effParams);

		// 初始化HatchShader
		hatchShader.addTexProperties("Hatch0", &hatchShader.properties.hatch0);
		hatchShader.addTexProperties("Hatch1", &hatchShader.properties.hatch1);
		hatchShader.addTexProperties("Hatch2", &hatchShader.properties.hatch2);
		hatchShader.addTexProperties("Hatch3", &hatchShader.properties.hatch3);
		hatchShader.addTexProperties("Hatch4", &hatchShader.properties.hatch4);
		hatchShader.addTexProperties("Hatch5", &hatchShader.properties.hatch5);
		hatchShader.addVecProperties("OutlineColor", &hatchShader.properties.outlineColor);
		hatchShader.addVecProperties("BaseColor", &hatchShader.properties.baseColor);
		hatchShader.addDoubleProperties("Outline", &hatchShader.properties.outline);
		hatchShader.addDoubleProperties("TileFactor", &hatchShader.properties.tileFactor);
	}
};

#endif