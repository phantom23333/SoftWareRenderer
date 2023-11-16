#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Common/geometry.h"

// 视锥体信息 假设z为正方向
struct _frustum {
	double aspect; // 长宽比
	double fov; // 垂直角度 弧度制
};

// 相机信息
class Camera {
private:
	double mDx = 0, mDy = 0;
	vec3 mPos;
	vec4 mRect; // (x, y, w, h)
	double mNear, mFar;
	_frustum mFrustum;

	mat<4, 4> mView, mProjection, mViewport;

	bool mDirtyFlag; // 更新标记

	vec3 L, R, U; // 观察向量 右向量 上向量

	double mRadius, mTheta, mPhi;

public:
	double minRadius = 1.0, maxRadius = 40.0;

private:
	// 计算视图矩阵
	void updateView();

	// 计算透视投影矩阵
	void updateProjection();

	// 计算视口矩阵
	void updateViewport();

public:
	Camera() = default;

	Camera(double radius, double theta, double phi, double aspect, double fov, double near, double far, vec4 rect);

	mat<4, 4> view() { return mView; }
	mat<4, 4> projection() { return mProjection; }
	mat<4, 4> viewport() { return mViewport; }

	double dis() const { return mFar - mNear; }

	void UpdateCoordinates();

	void lookAt(vec3 target);

	void rotated(double dTheta, double dPhi);
	void scaled(double dRadius);
	void moved(double dx, double dy);

	vec3 worldPos() { return{ mPos.x, -mPos.y, mPos.z }; }

	void updated();
};

#endif