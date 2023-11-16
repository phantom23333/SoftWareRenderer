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

// 将[0, 1]的浮点数depth编码到rbga 32bit存储编码 每一通道依次存储从左到右8位小数
// float本身32bit
vec4 BUFFER::pack(float depth) {
	// 使用rgba 4字节共32位来存储z值,1个字节精度为1/256
	const vec4 bitShift = vec4{ 1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0 };// 移位，每个通道二进制移n*8位
	const vec4 bitMask = vec4{ 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0, 0.0 };// 字节精度
	// gl_FragCoord:片元的坐标,fract():返回数值的小数部分
	vec4 rgbaDepth = frac(depth * bitShift); // 截掉每通道二进制小数点后n*8位数
	rgbaDepth = rgbaDepth - vec4{ rgbaDepth[1], rgbaDepth[1], rgbaDepth[2], rgbaDepth[2] } *bitMask; // 将每通道二进制位数限定在8bit（将每通道8bit后的截掉）
	return rgbaDepth;
}

// 将rgba解码到四通道
float BUFFER::unpack(vec4 rgbaDepth) {
	// 每一通道二进制右移8*n位
	const vec4 bitShift = vec4{ 1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0) };
	return dot(rgbaDepth, bitShift);
}