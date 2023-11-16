#ifndef __TEXTURE__H__
#define __TEXTURE__H__

#include <QImage>
#include <string>
#include <QFileInfo>
#include "geometry.h"

class Texture {
private:
	QImage* mTexture;
	std::string mFilepath;
	double mWidth, mHeight;
	vec2 mResolution;
	bool mIsNormal;

	QColor errorColor = QColor(218, 112, 214);
public:
	Texture(bool isNormal = false) : mWidth(1), mHeight(1), mResolution({ 1,1 }), mFilepath(""), mIsNormal(isNormal) {
		mTexture = new QImage(1, 1, QImage::Format_RGB888);
		if (isNormal)
			mTexture->setPixelColor(0, 0, QColor(125, 125, 125));
		else mTexture->setPixelColor(0, 0, errorColor);
	}

	Texture(int r, int g, int b, bool isNormal = false) : mWidth(1), mHeight(1), mResolution({ 1,1 }), mFilepath(""), mIsNormal(isNormal) {
		mTexture = new QImage(1, 1, QImage::Format_RGB888);
		mTexture->setPixelColor(0, 0, QColor(r, g, b));
	}

	Texture(std::string filepath) :mWidth(1.0), mHeight(1.0), mResolution({ 1.0, 1.0 }), mFilepath("") {
		mTexture = new QImage(mWidth, mHeight, QImage::Format_RGB888);
		loadTexture(filepath);
	}

	Texture(const Texture& t) {
		mFilepath = t.filepath();
		mWidth = t.width();
		mHeight = t.height();
		mResolution = t.resolution();
		mTexture = new QImage(t.img());
		mIsNormal = t.isNormal();
	}

	Texture(QImage* img) {
		mTexture = img;
		mWidth = img->width();
		mHeight = img->height();
		mResolution = { (double)img->width(), (double)img->height() };
		mIsNormal = false;
	}

	Texture& operator=(const Texture& other) {
		if (this == &other) {
			return *this;
		}
		mFilepath = other.filepath();
		mWidth = other.width();
		mHeight = other.height();
		mResolution = other.resolution();
		mTexture = new QImage(other.img());
		mIsNormal = other.isNormal();
		return *this;
	}

	~Texture() = default;

	bool loadTexture(std::string filepath) {
		if (!mTexture->load(filepath.c_str())) {
			mTexture = new QImage(1, 1, QImage::Format_RGB888);
			mTexture->setPixelColor(0, 0, errorColor);
			return false;
		}
		mFilepath = filepath;
		mWidth = (double)mTexture->width();
		mHeight = (double)mTexture->height();
		mResolution = { mWidth, mHeight };
		return true;
	}

	std::string filepath() const {
		return mFilepath;
	}

	double width() const {
		return mWidth;
	}

	double height() const {
		return mHeight;
	}

	vec2 resolution() const {
		return mResolution;
	}

	QColor get(const int& x, const int& y) const {
		if (x < 0 || x > mTexture->width() - 1 || y < 0 || y > mTexture->height() - 1) return errorColor;
		return mTexture->pixelColor(x, y);
	}

	QImage img() const {
		return *mTexture;
	}

	bool isNormal() const {
		return mIsNormal;
	}
};


#endif
