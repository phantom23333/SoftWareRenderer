#pragma once

#include <QtWidgets/QWidget>
#include "ui_rendererApp.h"
#include <vector>
#include <string>
#include <QApplication>
#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QThread>
#include <QKeyEvent>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QSignalMapper>
#include <QFileDialog>
#include <QStandardPaths>
#include <QAction>
#include <QMessagebox>
#include <QMenuBar>
#include <QComboBox>
#include "renderPipeline.h"
#include "light.h"
#include "ShaderManager.h"

class rendererApp : public QMainWindow
{
	Q_OBJECT

private:
	RenderPipeline mRenderPipeline;
	ShaderManager mShaderManager;

	int mShaderIdx = -1;
	int mModelIdx = -1;
	int mModelNum = 0;

	const int mMaxFps = 60;
public:
	rendererApp(QWidget* parent = nullptr);
	~rendererApp();

private:
	Ui::MainWindow ui;

	QLabel* mFpsLabel;
	QTimer* mTimer;

	std::vector<Model*> mModels;

	std::vector<double*> mDoubleProperties[SHADERMANAGER::number];
	std::vector<vec4*> mVecProperties[SHADERMANAGER::number];
	std::vector<Texture*> mTexProperties[SHADERMANAGER::number];

	int mIsMouseDown;
	QPoint mLastMousePos;

	double mFps;

	int mFenceCPU;
	int mFenceGPU;

	QString mLastPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

private:
	QLabel* mCanvas;

	QLineEdit* mLightRText, * mLightGText, * mLightBText;
	QLineEdit* mLightXText, * mLightYText, * mLightZText;
	QLineEdit* mAmbientRText, * mAmbientGText, * mAmbientBText;
	QPushButton* mLightButton;
	QLabel* mLightIntensityLabel;
	QSlider* mLightIntensitySlider;

	QScrollArea* mPropertiesArea = nullptr;
	QWidget* mPropertiesContainer = nullptr;
	QVBoxLayout* mVPropertiesLayout = nullptr;
	QPushButton* mPropertiesButton = nullptr;

	QComboBox* mModelBox;
	QComboBox* mCullBox;

	QLineEdit* mModelPosXText, * mModelPosYText, * mModelPosZText;
	QLineEdit* mModelScaleXText, * mModelScaleYText, * mModelScaleZText;
	QLineEdit* mModelRotationXText, * mModelRotationYText, * mModelRotationZText;

	QCheckBox* mZTestCheck, * mZWriteCheck, *mOverrideCheck;

	QMenuBar* mMenuBar;
	QMenu* mShaderMenu;
	QMenu* mModelMenu;
	QMenu* mDeleteModelMenu;
	QAction* mNewModelAction;


protected:
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

private:
	void init();
	void initWindow();
	void initApp();
	void initMenu();
	void captureWidget();
	void initWidget();

	void bindPropertiesUI();
	void bindPropertiesAddress();

	void render();

	void setOrder(std::initializer_list<QWidget*> list);
	std::string readImgFile();
	std::string readObjFile();

public slots:
	void updated();
	void onLightButtonClicked();
	void onLightIntensitySliderValueChange();
	void onPropertiesButtonClicked();
	void onPropertiesTextureButtonClicked(const int& i);
	void onPropertiesTextureClearButtonClicked(const int& i);
	void onNewModelActionTriggered();
	void onShaderActionTriggered(const int& i);
	void onModelBoxCurrentChanged(const int& i);
	void onZTestCheckedClicked(bool checked);
	void onZWriteCheckedClicked(bool checked);
	void onOverrideCheckClicked(bool checked);
	void onCullBoxCurrentChanged(const int &i);
	void onDeleteActionTriggerd(const int &i);


private:
	template<typename T>
	void bindPropertiesAddress(T& shader, int idx) {
		auto doubleProperties = shader.getDoubleProperties();
		auto vecProperties = shader.getVecProperties();
		auto texProperties = shader.getTexProperties();

		// 绑定double*
		for (auto t : doubleProperties) {
			double* address = t.second;
			mDoubleProperties[idx].push_back(address);
		}

		// 绑定vec4*
		for (auto t : vecProperties) {
			vec4* address = t.second;
			mVecProperties[idx].push_back(address);
		}

		// 绑定Texture*
		for (auto t : texProperties) {
			Texture* address = t.second;
			mTexProperties[idx].push_back(address);
		}
	}

	template<typename T>
	void bindPropertiesUI(T& shader) {
		if (mModelIdx == -1) return;
		auto model = mModels[mModelIdx];

		// 绑定Position
		vec3 pos = model->position();
		mModelPosXText->setText(QString::number(pos.x * 100.0));
		mModelPosYText->setText(QString::number(pos.y * 100.0));
		mModelPosZText->setText(QString::number(pos.z * 100.0));

		// 绑定Scale
		vec3 scaling = model->scaling();
		mModelScaleXText->setText(QString::number(scaling.x));
		mModelScaleYText->setText(QString::number(scaling.y));
		mModelScaleZText->setText(QString::number(scaling.z));

		// 绑定Rotation
		vec3 rotation = model->rotation();
		mModelRotationXText->setText(QString::number(rotation.x * 180.0 / MATH::PI));
		mModelRotationYText->setText(QString::number(rotation.y * 180.0 / MATH::PI));
		mModelRotationZText->setText(QString::number(rotation.z * 180.0 / MATH::PI));

		// 绑定ZTest
		mZTestCheck->setChecked(model->settings.zTest);
		// 绑定zWrite
		mZWriteCheck->setChecked(model->settings.zWrite);
		// 绑定override
		mOverrideCheck->setChecked(model->settings.override);
		// 绑定Cull
		mCullBox->setCurrentIndex(model->settings.cull);

		// 绑定Proerties
		if (mVPropertiesLayout != nullptr) {
			while (mVPropertiesLayout->count() > 0) {
				QLayout* layout = mVPropertiesLayout->itemAt(0)->layout();
				while (layout->count()) {
					QWidget* item = layout->itemAt(0)->widget();
					item->setParent(NULL);
					layout->removeWidget(item);
					delete item;
					item = nullptr;
				}
				mVPropertiesLayout->removeItem(layout);
			}
		}
		if (mShaderIdx == -1) return;

		auto doubleProperties = shader.getDoubleProperties();
		auto vecProperties = shader.getVecProperties();
		auto texProperties = shader.getTexProperties();

		QFont font = QApplication::font();
		font.setPixelSize(15);

		const int textWidth = 60, textHeight = 30;

		int pid = 0;
		// 绑定Texture属性
		int idx = 0;
		QSignalMapper* mapper = new QSignalMapper(this);
		QSignalMapper* mapper2 = new QSignalMapper(this);
		for (auto t : texProperties) {
			auto name = t.first;

			Texture* address = model->texProperties[mShaderIdx][pid++];

			QLabel* label = new QLabel(name.c_str(), mPropertiesArea);
			label->setFont(font);

			QImage img = address->img();
			QLabel* canvas = new QLabel("Texture Canvas", mPropertiesArea);
			canvas->setFixedSize(60, 60);
			canvas->setPixmap(QPixmap::fromImage(img).scaled(canvas->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

			QFont bfont = QApplication::font();
			bfont.setPointSize(8);

			QPushButton* button = new QPushButton("...", mPropertiesArea);
			button->setFixedSize(30, 20);
			button->setFont(bfont);
			connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
			mapper->setMapping(button, idx);

			QPushButton* button2 = new QPushButton("clear", mPropertiesArea);
			button2->setFixedSize(40, 20);
			button2->setFont(bfont);
			connect(button2, SIGNAL(clicked()), mapper2, SLOT(map()));
			mapper2->setMapping(button2, idx++);

			QHBoxLayout* layout = new QHBoxLayout();
			layout->setAlignment(Qt::AlignLeft);
			layout->setSpacing(15);
			layout->addWidget(label);
			layout->addWidget(canvas);
			layout->addWidget(button);
			layout->addWidget(button2);

			mVPropertiesLayout->addLayout(layout);
		}
		connect(mapper, SIGNAL(mappedInt(int)), this, SLOT(onPropertiesTextureButtonClicked(int)));
		connect(mapper2, SIGNAL(mappedInt(int)), this, SLOT(onPropertiesTextureClearButtonClicked(int)));

		// 绑定double属性
		pid = 0;
		for (auto t : doubleProperties) {
			auto name = t.first;
			double* address = model->doubleProperties[mShaderIdx][pid++];

			QLabel* label = new QLabel(name.c_str(), mPropertiesArea);
			label->setFont(font);

			QLineEdit* text = new QLineEdit();
			text->setFixedSize(textWidth, textHeight);
			text->setText(QString::number(*address));
			text->setFont(font);
			connect(text, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));

			QHBoxLayout* layout = new QHBoxLayout();
			layout->setAlignment(Qt::AlignLeft);
			layout->addWidget(label);
			layout->addWidget(text);

			mVPropertiesLayout->addLayout(layout);
		}

		// 绑定vec4属性
		pid = 0;
		for (auto t : vecProperties) {
			auto name = t.first;
			vec4* address = model->vecProperties[mShaderIdx][pid++];

			QLabel* label = new QLabel(name.c_str(), mPropertiesArea);
			label->setFont(font);

			QHBoxLayout* layout = new QHBoxLayout();
			layout->setAlignment(Qt::AlignLeft);
			layout->addWidget(label);
			for (int i = 0; i < 4; i++) {
				QLineEdit* text = new QLineEdit();
				text->setFixedSize(textWidth, textHeight);
				text->setFont(font);
				text->setText(QString::number((*address)[i]));
				connect(text, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));

				layout->addWidget(text);
			}

			mVPropertiesLayout->addLayout(layout);
		}

		mPropertiesContainer->setLayout(mVPropertiesLayout);
		mPropertiesArea->setWidget(mPropertiesContainer);
	}

};
