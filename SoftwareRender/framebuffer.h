#ifndef __FRAMEBUFFER__H__
#define __FRAMEBUFFER__H__

#include <QImage>
#include <QLabel>
#include <cmath>
#include "Common/geometry.h"

namespace BUFFER {
	vec4 pack(float depth);
	float unpack(vec4 rgbaDepth);

}

class ColorBuffer {
public:
	ColorBuffer() = default;
	ColorBuffer(int width, int height);

	void clear(vec3 color);
	void set(int x, int y, vec3 color);

	void showOnLabel(QLabel* canvas);

	QImage* buffer() { return mBuffer; }

	int height() { return mHeight; }
	int width() { return mWidth; }

public:
	QImage* mBuffer;
	int mWidth, mHeight;
};


class DepthBuffer {
public:
	DepthBuffer() = default;
	DepthBuffer(int width, int height);

	void clear();
	void set(int x, int y, double depth);
	double get(int x, int y);

	void showOnLabel(QLabel* canvas);

	QImage* buffer() { return mBuffer; }

	int height() { return mHeight; }
	int width() { return mWidth; }


public:
	QImage* mBuffer;
	int mWidth, mHeight;
};

class FrameBuffer {
public:
	FrameBuffer() = default;
	FrameBuffer(int width, int height);
	ColorBuffer colorBuffer() { return mColorBuffer; }
	DepthBuffer depthBuffer() { return mDepthBuffer; }

private:
	ColorBuffer mColorBuffer;
	DepthBuffer mDepthBuffer;
};
#endif