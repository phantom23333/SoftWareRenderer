#include "renderer.h"

vec3 Renderer::barycentric(vec4* pts, vec3 p) {
	vec3 v1 = { pts[1].x - pts[0].x, pts[2].x - pts[0].x, pts[0].x - p.x }, v2 = { pts[1].y - pts[0].y, pts[2].y - pts[0].y, pts[0].y - p.y };
	vec3 t = cross(v1, v2);
	// �����˽����z����Ϊ0��������Ϊ��һ������
	if (std::abs(t.z) < 1e-2) return { -1, 1, 1 }; // ���ظ����꣬�ڹ�դ��������
	return { 1.0 - (t.x + t.y) / t.z, t.x / t.z, t.y / t.z }; // ����ϵ��
}

double Renderer::interpolate(const vec3& val, const vec3& weight) {
	double res = 0;
	for (int i = 0; i < 3; i++) res += val[i] * weight[i];
	return res;
}
