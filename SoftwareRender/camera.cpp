#include "camera.h"

Camera::Camera(double radius, double theta, double phi, double aspect, double fov, double near, double far, vec4 rect) : mRadius(radius), mTheta(theta), mPhi(phi), mNear(near), mFar(far), mRect(rect) {
	mFrustum.aspect = aspect;
	mFrustum.fov = fov;

	UpdateCoordinates();
	mDirtyFlag = true;
}

void Camera::UpdateCoordinates() {
	double x = mDx + mRadius * sinf(mPhi) * cosf(mTheta);
	double z = mRadius * sinf(mPhi) * sinf(mTheta);
	double y = mDy + mRadius * cosf(mPhi);
	mPos = { x, y, -z };// local pos inverse world pos z
}

void Camera::lookAt(vec3 target) {
	L = (target - mPos).normalized();
	R = cross({ 0, 1, 0 }, L).normalized(); // 右向量
	U = cross(L, R).normalized(); // 上向量

	mDirtyFlag = true;
}

void Camera::updateView() {
	L = L.normalized(); // 观察方向
	U = cross(L, R).normalized(); // 上向量
	R = cross(L, U).normalized(); // 右向量

	auto& v = mView;
	v = mat<4, 4>::identity();
	v[0][0] = R.x;
	v[0][1] = R.y;
	v[0][2] = R.z;
	v[0][3] = -mPos.x * R.x - mPos.y * R.y - mPos.z * R.z;
	v[1][0] = U.x;
	v[1][1] = U.y;
	v[1][2] = U.z;
	v[1][3] = -mPos.x * U.x - mPos.y * U.y - mPos.z * U.z;
	v[2][0] = -L.x;
	v[2][1] = -L.y;
	v[2][2] = -L.z;
	v[2][3] = mPos.x * L.x + mPos.y * L.y + mPos.z * L.z;
}

void Camera::updateProjection() {
	auto& p = mProjection;
	p = mat<4, 4>::identity();
	p[0][0] = 1.0 / tan(mFrustum.fov / 2.0) * mFrustum.aspect;
	p[1][1] = 1.0 / tan(mFrustum.fov / 2.0);
	p[2][2] = (mNear + mFar) / (mNear - mFar);
	p[2][3] = (2 * mNear * mFar) / (mNear - mFar);
	p[3][2] = -1;
	p[3][3] = 0;
}

void Camera::updateViewport() {
	auto& v = mViewport;
	v = mat<4, 4>::identity();
	v[0][0] = mRect[2] / 2.0;
	v[0][3] = mRect[2] / 2.0;
	v[1][1] = mRect[3] / 2.0 + mRect[0];
	v[1][3] = mRect[3] / 2.0 + mRect[1];
}

void Camera::updated() {
	if (!mDirtyFlag) return;
	UpdateCoordinates();
	lookAt({ mDx,mDy ,0 });
	updateView();
	updateProjection();
	updateViewport();
}

void Camera::rotated(double dTheta, double dPhi) {
	mPhi += dPhi;
	mTheta += dTheta;
	mPhi = clamp(mPhi, 0.1f, 3.14f - 0.1f);
	mDirtyFlag = true;
}

void Camera::scaled(double dRadius) {
	mRadius += dRadius;
	mRadius = clamp(mRadius, minRadius, maxRadius);
}

void Camera::moved(double dx, double dy) {
	mDx += dx;
	mDy += dy;
}