#include "renderPipeline.h"

mat<4, 4> RenderPipeline::calcLightMVP(const vec3& translate, const vec3& scale) {
	// 计算世界空间到光源空间的变换矩阵
	using mat4 = mat<4, 4>;
	mat4 modelMat = mat4::identity(), viewMat = mat4::identity(), projMat;
	for (int i = 0; i < 3; i++) {
		modelMat[i][i] = scale[i];
		modelMat[i][3] = translate[i];
	}

	viewMat = lookAt(mLight.direction, { 0, 0, 0 }, { 0, 1, 0 });

	const double bound = 3, near = 0.1, far = 1;
	projMat[0][0] = 1.0 / bound;
	projMat[1][1] = 1.0 / bound;
	projMat[2][2] = 2.0 / near - far;
	projMat[2][3] = (near + far) / (far - near);
	projMat[3][3] = 1;

	return projMat * viewMat * modelMat;
}

RenderPipeline::RenderPipeline(int width, int height) : mWidth(width), mHeight(height) {
	mGameTimer = GameTimer();
	mRenderer = Renderer(width, height);
	mFrameBuffer = FrameBuffer(width, height);

	mCamera = Camera(8, 1.5 * 3.14, 0.7853, 1.77, 1.04, 0.01, 1000, { 0, 0, (double)width, (double)height });
	mCamera.lookAt({ 0, 0, 0 });
	mCamera.updated();

	mLight = Light({ 0.8, 0.8, 0.8 }, { 1, -1, std::sqrt(3) }, { 0.1, 0.1, 0.1 }, 1);
}

void RenderPipeline::setLightColor(double r, double g, double b) {
	mLight.color = { r / 255.0, g / 255.0, b / 255.0 };
}

void RenderPipeline::setLightIntensity(double intensity) {
	mLight.intensity = intensity;
}

void RenderPipeline::setLightDirection(vec3 direction) {
	direction = direction.normalized();
	mLight.direction = direction;
}

void RenderPipeline::setAmbient(double r, double g, double b) {
	mLight.ambient = { r / 255.0, g / 255.0, b / 255.0 };
}
