#include "framebuffer.h"

ColorBuffer::ColorBuffer(int width, int height) : mWidth(width), mHeight(height) {
	mBuffer = new QImage(width, height, QImage::Format_RGB888);
}

void ColorBuffer::clear(vec3 color) {
	QColor tmp = QColor(255.0 * color.x, 255.0 * color.y, 255.0 * color.z);
	mBuffer->fill(tmp);
}

void ColorBuffer::set(int x, int y, vec3 color) {
	color = saturate(color) * 255;
	QColor finalColor = QColor(color.x, color.y, color.z);
	mBuffer->setPixelColor(x, mHeight - y - 1, finalColor);
}

void ColorBuffer::showOnLabel(QLabel* canvas) {
	canvas->setPixmap(QPixmap::fromImage(*mBuffer));
}

DepthBuffer::DepthBuffer(int width, int height) : mWidth(width), mHeight(height) {
	mBuffer = new QImage(width, height, QImage::Format_Grayscale16);
	mBuffer->setColorCount(65536);
}

void DepthBuffer::set(int x, int y, double depth) {
	vec4 rgba = BUFFER::pack(depth);
	mBuffer->setPixel(x, mHeight - y - 1, qRgba(rgba.x, rgba.y, rgba.z, rgba.w));
}

double DepthBuffer::get(int x, int y) {
	auto rgba = mBuffer->pixel(x, mHeight - y - 1);
	double r = qRed(rgba);
	double g = qGreen(rgba);
	double b = qBlue(rgba);
	double a = qAlpha(rgba);

	double depth = BUFFER::unpack(vec4{ r, g, b, a });
	return depth;
}

void DepthBuffer::showOnLabel(QLabel* canvas) {
	canvas->setPixmap(QPixmap::fromImage(*mBuffer));
}

void DepthBuffer::clear() {
	mBuffer->fill(qRgba(255, 255, 255, 255));
}

FrameBuffer::FrameBuffer(int width, int height) {
	mColorBuffer = ColorBuffer(width, height);
	mDepthBuffer = DepthBuffer(width, height);
}

// ��[0, 1]�ĸ�����depth���뵽rbga 32bit�洢���� ÿһͨ�����δ洢������8λС��
// float����32bit
vec4 BUFFER::pack(float depth) {
	// ʹ��rgba 4�ֽڹ�32λ���洢zֵ,1���ֽھ���Ϊ1/256
	const vec4 bitShift = vec4{ 1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0 };// ��λ��ÿ��ͨ����������n*8λ
	const vec4 bitMask = vec4{ 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0, 0.0 };// �ֽھ���
	// gl_FragCoord:ƬԪ������,fract():������ֵ��С������
	vec4 rgbaDepth = frac(depth * bitShift); // �ص�ÿͨ��������С�����n*8λ��
	rgbaDepth = rgbaDepth - vec4{ rgbaDepth[1], rgbaDepth[1], rgbaDepth[2], rgbaDepth[2] } *bitMask; // ��ÿͨ��������λ���޶���8bit����ÿͨ��8bit��Ľص���
	return rgbaDepth;
}

// ��rgba���뵽��ͨ��
float BUFFER::unpack(vec4 rgbaDepth) {
	// ÿһͨ������������8*nλ
	const vec4 bitShift = vec4{ 1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0) };
	return dot(rgbaDepth, bitShift);
}