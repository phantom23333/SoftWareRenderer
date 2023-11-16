#include <iostream>
#include <sstream>
#include "model.h"

Model::Model(const std::string filename) {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			vec3 v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts.push_back(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			vec3 n;
			for (int i = 0; i < 3; i++) iss >> n[i];
			norms.push_back(n.normalized());
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			vec2 uv;
			for (int i = 0; i < 2; i++) iss >> uv[i];
			tex_coord.push_back({ uv.x, 1 - uv.y });
		}
		else if (!line.compare(0, 2, "f ")) {
			int f, t, n;
			iss >> trash;
			int cnt = 0;
			while (iss >> f >> trash >> t >> trash >> n) {
				facet_vrt.push_back(--f);
				facet_tex.push_back(--t);
				facet_nrm.push_back(--n);
				cnt++;
			}
			if (3 != cnt) {
				std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
				return;
			}
		}
	}
	// 计算tangent
	if (!verts.empty() && !norms.empty() && !tex_coord.empty()) {
		// 遍历每个三角形
		for (int i = 0; i < facet_vrt.size(); i += 3) {
			vec4 tangent;

			vec3 p1 = verts[facet_vrt[i]], p2 = verts[facet_vrt[i + 1]], p3 = verts[facet_vrt[i + 2]];
			vec2 uv1 = tex_coord[facet_tex[i]], uv2 = tex_coord[facet_tex[i + 1]], uv3 = tex_coord[facet_tex[i + 2]];
			vec3 e1 = p2 - p1, e2 = p3 - p1;
			vec2 dUV1 = uv2 - uv1, dUV2 = uv3 - uv1;

			// 分数部分
			double tmp = dUV1.x * dUV2.y - dUV2.x * dUV1.y;
			// 防止分母为0！重要
			if (tmp < 1e-7) tmp = 1e-7;
			double f = 1.0 / tmp;

			tangent.x = f * (dUV2.y * e1.x - dUV1.y * e2.x);
			tangent.y = f * (dUV2.y * e1.y - dUV1.y * e2.y);
			tangent.z = f * (dUV2.y * e1.z - dUV1.y * e2.z);
			tangent = tangent.normalized();

			tangents.push_back(tangent);
		}
	}
	else {
		std::cerr << "Cannot calculate tangent !" << std::endl;
	}
	std::cerr << "# v# " << nverts() << " f# " << nfaces() << " vt# " << tex_coord.size() << " vn# " << norms.size() << std::endl;
}

int Model::nverts() const {
	return verts.size();
}

int Model::nfaces() const {
	return facet_vrt.size() / 3;
}

vec3 Model::vert(const int i) const {
	return verts[i];
}

vec3 Model::vert(const int iface, const int nthvert) const {
	return verts[facet_vrt[iface * 3 + nthvert]];
}

vec2 Model::uv(const int iface, const int nthvert) const {
	return tex_coord[facet_tex[iface * 3 + nthvert]];
}


vec3 Model::normal(const int iface, const int nthvert) const {
	return norms[facet_nrm[iface * 3 + nthvert]];
}

vec4 Model::tangent(const int iface) const {
	assert(iface >= 0 && iface < tangents.size());
	return tangents[iface];
}

void Model::scaled(double sx, double sy, double sz) {
	mModelMat[0][0] *= sx;
	mModelMat[1][1] *= sy;
	mModelMat[2][2] *= sz;

	mScaling = { sx, sy, sz };
}

void Model::moved(double mx, double my, double mz) {
	mModelMat[0][3] += mx;
	mModelMat[1][3] += my;
	mModelMat[2][3] += mz;

	mPos = { mx, my, mz };
}

void Model::rotated(double rx, double ry, double rz) {
	mModelMat = mat<4, 4>::identity();

	// x-axis rotation
	double sx = sin(rx);
	double cx = cos(rx);
	// y-axis rotation
	double sy = sin(ry);
	double cy = cos(ry);
	// z-axis rotation
	double sz = sin(rz);
	double cz = cos(rz);

	// create rotation matrix
	mat<4, 4> rotation;
	rotation[0][0] = cy * cz;
	rotation[0][1] = -cy * sz;
	rotation[0][2] = sy;
	rotation[1][0] = cx * sz + cz * sx * sy;
	rotation[1][1] = cx * cz - sx * sy * sz;
	rotation[1][2] = -cy * sx;
	rotation[2][0] = sx * sz - cx * cz * sy;
	rotation[2][1] = cz * sx + cx * sy * sz;
	rotation[2][2] = cx * cy;
	rotation[3][3] = 1;

	// apply rotation
	mModelMat = rotation;

	mRot = { rx, ry, rz };
}