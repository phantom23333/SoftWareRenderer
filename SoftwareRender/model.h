#ifndef __MODEL__H__
#define __MODEL__H__

#include <vector>
#include <string>
#include <fstream>
#include "ShaderManager.h"
#include "Common/geometry.h"
#include "Common/texture.hpp"

class Model {
private:
	std::vector<vec3> verts{};     // array of vertices
	std::vector<vec2> tex_coord{}; // per-vertex array of tex coords
	std::vector<vec3> norms{};     // per-vertex array of normal vectors
	std::vector<vec3> tangents{};
	std::vector<int> facet_vrt{};
	std::vector<int> facet_tex{};  // per-triangle indices in the above arrays
	std::vector<int> facet_nrm{};

	mat<4, 4> mModelMat = mat<4, 4>::identity();
	vec3 mPos = { 0, 0, 0 };
	vec3 mScaling = { 1, 1, 1 };
	vec3 mRot = { 0, 0, 0 };

public:
	std::vector<double*> doubleProperties[SHADERMANAGER::number];
	std::vector<vec4*> vecProperties[SHADERMANAGER::number];
	std::vector<Texture*> texProperties[SHADERMANAGER::number];

	struct Settings {
		bool override = false;// 是否以模型为主 而非Shader
		bool zTest = true;
		bool zWrite = true;
		int cull = 0;// 0 cull back 1 cull froint 2 cull none
	}settings;


public:
	Model(const std::string filename);
	int nverts() const;
	int nfaces() const;
	vec3 normal(const int iface, const int nthvert) const; // per triangle corner normal vertex
	vec3 vert(const int i) const;
	vec3 vert(const int iface, const int nthvert) const;
	vec2 uv(const int iface, const int nthvert) const;
	vec4 tangent(const int iface) const;

	void scaled(double sx, double sy, double sz);
	void moved(double mx, double my, double mz);
	void rotated(double rx, double ry, double rz);// 角度制

	mat<4, 4> modelMat() { return mModelMat; }

	vec3 position() { return mPos; }
	vec3 scaling() { return mScaling; }
	vec3 rotation() { return mRot; }

};

#endif