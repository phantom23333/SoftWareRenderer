#ifndef __SHADER_CORE__H__
#define __SHADER_CORE__H__

#include <cmath>
#include <QColor>
#include "../Common/geometry.h"
#include "../Common/texture.hpp"

inline vec4 tex2D(const Texture& map, vec2 uv) {
	vec4 res;
	QColor var_map = map.get(int(frac(uv.x) * map.width()), int(frac(uv.y) * map.height()));
	res[0] = var_map.red() / 255.0;
	res[1] = var_map.green() / 255.0;
	res[2] = var_map.blue() / 255.0;
	res[3] = var_map.alpha() / 255.0;
	return res;
}

inline vec3 unpackNormal(vec3 packednormal, int scale = 1) {
	vec3 normal;
	normal = (packednormal.xy() * 2 - 1) * scale;
	normal.z = sqrt(1 - saturate(dot(normal.xy(), normal.xy())));
	return normal;
}


inline double min(const double& x, const double& y) {
	return std::min(x, y);
}

template<int n> vec<n> min(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> res = lhs;
	for (int i = 0; i < n; i++) res[i] = std::min(lhs[i], rhs[i]);
	return res;
}
inline double max(const double& x, const double& y) {
	return std::max(x, y);
}

template<int n> vec<n> max(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> res = lhs;
	for (int i = 0; i < n; i++) res[i] = std::max(lhs[i], rhs[i]);
	return res;
}

#define clip(x)\
	if((x) < 0)\
		return vec4{-233, -233, -233, -233};


#endif