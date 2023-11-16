#include "rendererApp.h"

rendererApp::rendererApp(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	init();
}

rendererApp::~rendererApp()
{}


// ������
// ��������������Ҫ����Shader������˳����е���
// ������

// ���԰󶨵�ַ
void rendererApp::bindPropertiesAddress() {
	bindPropertiesAddress(mShaderManager.phongShader, 0);
	bindPropertiesAddress(mShaderManager.rampShader, 1);
	bindPropertiesAddress(mShaderManager.animShader, 2);
	bindPropertiesAddress(mShaderManager.hatchShader, 3);
}

// ���԰�GUI
void rendererApp::bindPropertiesUI() {
	switch (mShaderIdx) {
	case 0:
		bindPropertiesUI(mShaderManager.phongShader);
		break;
	case 1:
		bindPropertiesUI(mShaderManager.rampShader);
		break;
	case 2:
		bindPropertiesUI(mShaderManager.animShader);
		break;
	case 3:
		bindPropertiesUI(mShaderManager.hatchShader);
		break;
	default:
		bindPropertiesUI(mShaderManager.errorShader);
		break;
	}
}

// ��Ⱦ
void rendererApp::render() {
	switch (mShaderIdx) {
	case 0:
		mFps = mRenderPipeline.render(mModels, &mShaderManager.phongShader, mShaderIdx, mFenceGPU);
		break;
	case 1:
		mFps = mRenderPipeline.render(mModels, &mShaderManager.rampShader, mShaderIdx, mFenceGPU);
		break;
	case 2:
		mFps = mRenderPipeline.render(mModels, &mShaderManager.animShader, mShaderIdx, mFenceGPU);
		break;
	case 3:
		mFps = mRenderPipeline.render(mModels, &mShaderManager.hatchShader, mShaderIdx, mFenceGPU);
		break;
	default:
		mFps = mRenderPipeline.render(mModels, &mShaderManager.errorShader, -1, mFenceGPU);
		break;
	}
}

void rendererApp::init() {
	captureWidget();
	initWindow();
	initApp();
	initMenu();
	initWidget();

	// ����ö��߳�(?) ������ûŪ������QThread����ô���÷�
	mTimer = new QTimer(this);
	connect(mTimer, &QTimer::timeout, [this]() {
		if (mFenceCPU < mFenceGPU) return;
		updated();
		});
	mTimer->start(1000 / mMaxFps);
}

// ��ʼ������
void rendererApp::initWindow() {
	// �������� https://github.com/GTRONICK/QSS
	QFile styleSheetFile("stylesheet templates/Aqua.qss");
	styleSheetFile.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(styleSheetFile.readAll());
	this->setStyleSheet(styleSheet);

	// ����FPS��ǩ
	mFpsLabel = new QLabel(mCanvas);
	mFpsLabel->setGeometry(10, 15, 30, 40);
	QPalette palette = mFpsLabel->palette();
	palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
	mFpsLabel->setPalette(palette);
	QFont font = mFpsLabel->font();
	font.setPointSize(20);
	mFpsLabel->setFont(font);
	mFpsLabel->setTabletTracking(false);
}

// ��ʼ������
void rendererApp::initApp() {
	mRenderPipeline = RenderPipeline(mCanvas->width(), mCanvas->height());

	// ������
	mShaderManager.initShader();
	bindPropertiesAddress();

	mFenceCPU = mFenceGPU = 0;
}

// �����˵�
void rendererApp::initMenu() {
	QSignalMapper* mapper = new QSignalMapper(this);
	for (int i = 0; i < SHADERMANAGER::number; i++) {
		QAction* action = new QAction(SHADERMANAGER::names[i].c_str(), mShaderMenu);
		connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
		mapper->setMapping(action, i);
		mShaderMenu->addAction(action);
	}

	QAction* action = new QAction("Clear", mShaderMenu);
	connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
	mapper->setMapping(action, -1);
	mShaderMenu->addAction(action);

	connect(mapper, SIGNAL(mappedInt(int)), this, SLOT(onShaderActionTriggered(int)));

	// ����delteModel
	mDeleteModelMenu = new QMenu("Delete");
	mModelMenu->addMenu(mDeleteModelMenu);
}

// ��ȡui�еĿؼ�
void rendererApp::captureWidget() {
	// ���ڿؼ�
	mCanvas = ui.label_canvas;

	mLightRText = ui.lineEdit_light_r;
	mLightGText = ui.lineEdit_light_g;
	mLightBText = ui.lineEdit_light_b;

	mLightXText = ui.lineEdit_light_x;
	mLightYText = ui.lineEdit_light_y;
	mLightZText = ui.lineEdit_light_z;

	mAmbientRText = ui.lineEdit_ambient_r;
	mAmbientGText = ui.lineEdit_ambient_g;
	mAmbientBText = ui.lineEdit_ambient_b;

	mLightButton = ui.pushButton_light;

	mLightIntensitySlider = ui.slider_light_intensity;
	mLightIntensityLabel = ui.label_light_intensity2;

	mPropertiesArea = ui.scrollArea_properties;
	mPropertiesContainer = ui.scrollAreaWidgetContents_properties;
	mVPropertiesLayout = ui.verticalLayout_properties;
	mPropertiesButton = ui.pushButton_properties;
	mModelBox = ui.comboBox_model;

	mModelPosXText = ui.lineEdit_modelPos_x;
	mModelPosYText = ui.lineEdit_modelPos_y;
	mModelPosZText = ui.lineEdit_modelPos_z;
	mModelScaleXText = ui.lineEdit_modelScale_x;
	mModelScaleYText = ui.lineEdit_modelScale_y;
	mModelScaleZText = ui.lineEdit_modelScale_z;
	mModelRotationXText = ui.lineEdit_modelRotation_x;
	mModelRotationYText = ui.lineEdit_modelRotation_y;
	mModelRotationZText = ui.lineEdit_modelRotation_z;

	mCullBox = ui.comboBox_cull;

	mZTestCheck = ui.checkBox_zTest;
	mZWriteCheck = ui.checkBox_zWrite;
	mOverrideCheck = ui.checkBox_override;

	// �˵���
	mMenuBar = ui.menubar;
	mModelMenu = ui.menu_model;
	mShaderMenu = ui.menu_shader;
	mNewModelAction = ui.action_newmodel;

}

// ��ʼ���ؼ�
void rendererApp::initWidget() {
	// ��ʼ����ʽ
	QDoubleValidator* rgbValidator = new QDoubleValidator(0.0, 255.0, 2, this);
	mLightRText->setValidator(rgbValidator);
	mLightGText->setValidator(rgbValidator);
	mLightBText->setValidator(rgbValidator);
	mAmbientRText->setValidator(rgbValidator);
	mAmbientGText->setValidator(rgbValidator);
	mAmbientBText->setValidator(rgbValidator);

	Light light = mRenderPipeline.light();
	mLightRText->setText(QString::number(light.color.x * 255));
	mLightGText->setText(QString::number(light.color.y * 255));
	mLightBText->setText(QString::number(light.color.z * 255));
	mLightXText->setText(QString::number(light.direction.x));
	mLightYText->setText(QString::number(light.direction.y));
	mLightZText->setText(QString::number(light.direction.z));
	mAmbientRText->setText(QString::number(light.ambient.x * 255));
	mAmbientGText->setText(QString::number(light.ambient.y * 255));
	mAmbientBText->setText(QString::number(light.ambient.z * 255));
	mLightIntensitySlider->setValue(light.intensity * 10);

	mVPropertiesLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	QDoubleValidator* doubleValidator = new QDoubleValidator(this);
	mModelPosXText->setValidator(doubleValidator);
	mModelPosYText->setValidator(doubleValidator);
	mModelScaleXText->setValidator(doubleValidator);
	mModelScaleYText->setValidator(doubleValidator);
	mModelScaleZText->setValidator(doubleValidator);
	mModelRotationXText->setValidator(doubleValidator);
	mModelRotationYText->setValidator(doubleValidator);
	mModelRotationZText->setValidator(doubleValidator);

	// signals&slots
	connect(mLightRText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mLightGText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mLightBText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mLightXText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mLightYText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mLightZText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mAmbientRText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mAmbientGText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mAmbientBText, SIGNAL(returnPressed()), mLightButton, SLOT(click()));
	connect(mLightButton, SIGNAL(clicked()), this, SLOT(onLightButtonClicked()));
	connect(mLightIntensitySlider, SIGNAL(valueChanged(int)), this, SLOT(onLightIntensitySliderValueChange()));
	connect(mPropertiesButton, SIGNAL(clicked()), this, SLOT(onPropertiesButtonClicked()));

	connect(mNewModelAction, SIGNAL(triggered()), this, SLOT(onNewModelActionTriggered()));

	connect(mModelBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onModelBoxCurrentChanged(int)));

	connect(mModelPosXText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));
	connect(mModelPosYText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));
	connect(mModelPosZText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));
	connect(mModelRotationXText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));
	connect(mModelRotationYText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));
	connect(mModelRotationZText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));
	connect(mModelScaleXText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));
	connect(mModelScaleYText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));
	connect(mModelScaleZText, SIGNAL(returnPressed()), mPropertiesButton, SLOT(click()));

	connect(mZTestCheck, SIGNAL(clicked(bool)), this, SLOT(onZTestCheckedClicked(bool)));
	connect(mZWriteCheck, SIGNAL(clicked(bool)), this, SLOT(onZWriteCheckedClicked(bool)));
	connect(mOverrideCheck, SIGNAL(clicked(bool)), this, SLOT(onOverrideCheckClicked(bool)));

	connect(mCullBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCullBoxCurrentChanged(int)));

}

// Updated
void rendererApp::updated() {
	mFenceCPU++;

	render();

	mFpsLabel->setText(QString::number(int(mFps)));

	QImage* buffer = mRenderPipeline.framebuffer().colorBuffer().mBuffer;
	mRenderPipeline.framebuffer().colorBuffer().showOnLabel(mCanvas);

	update();
}

// ����ƶ�
void rendererApp::mouseMoveEvent(QMouseEvent* event) {
	// ����ڻ������ƶ�
	if (mCanvas->rect().contains(event->pos())) {
		auto& camera = mRenderPipeline.camera();
		// ��������
		if (mIsMouseDown == 1) {
			if (mCanvas->cursor() != Qt::CrossCursor) {
				mCanvas->setCursor(Qt::CrossCursor);
			}

			// �����ƶ����벢ת��Ϊ���� 0.25Ϊ������ֵ
			double dx = (mLastMousePos.x() - event->pos().x()) * 3.14 / 180.0 * 0.25;
			double dy = (mLastMousePos.y() - event->pos().y()) * 3.14 / 180.0 * 0.25;

			// ����������ڻ���
			camera.rotated(dx, dy);

			mLastMousePos = event->pos();
		}
		// ������Ҽ�
		else if (mIsMouseDown == -1) {
			if (mCanvas->cursor() != Qt::OpenHandCursor) {
				mCanvas->setCursor(Qt::OpenHandCursor);
			}

			double dx = 0.0002 * (event->pos().x() - mLastMousePos.x());
			double dy = 0.0002 * (event->pos().y() - mLastMousePos.y());

			// �����������λ��
			camera.moved(dx, -dy);
		}

	}
}

// ��갴��
void rendererApp::mousePressEvent(QMouseEvent* event) {
	// ����ڻ����ϰ���
	if (mCanvas->rect().contains(event->pos())) {
		if (event->button() == Qt::LeftButton)
			mIsMouseDown = 1;
		if (event->button() == Qt::RightButton)
			mIsMouseDown = -1;

		mLastMousePos = event->pos();
	}
}

// ����ɿ�
void rendererApp::mouseReleaseEvent(QMouseEvent* event) {
	if (mIsMouseDown) {
		mIsMouseDown = 0;
		mCanvas->unsetCursor();
	}
}

// ������
void rendererApp::wheelEvent(QWheelEvent* event) {
	if (mCanvas->rect().contains(event->position().x(), event->position().y())) {
		auto& camera = mRenderPipeline.camera();
		double delta = event->angleDelta().y() * 0.005;

		camera.scaled(-delta);
	}
}

// �ƹ�����
void rendererApp::onLightButtonClicked() {
	// ����light��ɫ
	double r = mLightRText->text().toDouble();
	double g = mLightGText->text().toDouble();
	double b = mLightBText->text().toDouble();

	if (r > 255.0) {
		r = 255.0;
		mLightRText->setText(QString::number(255));
	}
	if (g > 255.0) {
		g = 255.0;
		mLightGText->setText(QString::number(255));
	}
	if (b > 255.0) {
		b = 255.0;
		mLightBText->setText(QString::number(255));
	}

	mRenderPipeline.setLightColor(r, g, b);

	// ����light����
	double x = mLightXText->text().toDouble();
	double y = mLightYText->text().toDouble();
	double z = mLightZText->text().toDouble();
	vec3 dir{ x, y ,z };
	dir = dir.normalized();
	mRenderPipeline.setLightDirection(dir);

	mLightXText->setText(QString::number(dir.x));
	mLightYText->setText(QString::number(dir.y));
	mLightZText->setText(QString::number(dir.z));

	// ����ambient
	r = mAmbientRText->text().toDouble();
	g = mAmbientGText->text().toDouble();
	b = mAmbientBText->text().toDouble();
	if (r > 255.0) {
		r = 255.0;
		mAmbientRText->setText(QString::number(255));
	}
	if (g > 255.0) {
		g = 255.0;
		mAmbientGText->setText(QString::number(255));
	}
	if (b > 255.0) {
		b = 255.0;
		mAmbientBText->setText(QString::number(255));
	}
	mRenderPipeline.setAmbient(r, g, b);
}

// ���ĵƹ�ǿ��
void rendererApp::onLightIntensitySliderValueChange() {
	double intensity = mLightIntensitySlider->value() / 10.0;
	mRenderPipeline.setLightIntensity(intensity);
	mLightIntensityLabel->setText(QString::number(intensity));
}

// ȷ��Properties
void rendererApp::onPropertiesButtonClicked() {
	auto& model = mModels[mModelIdx];

	// rotation
	double rx = mModelRotationXText->text().toDouble(), ry = mModelRotationYText->text().toDouble(), rz = mModelRotationZText->text().toDouble();
	rx = rx * MATH::PI / 180.0;
	ry = ry * MATH::PI / 180.0;
	rz = rz * MATH::PI / 180.0;
	model->rotated(rx, ry, rz);

	// position
	double px = mModelPosXText->text().toDouble(), py = mModelPosYText->text().toDouble(), pz = mModelPosZText->text().toDouble();
	model->moved(px / 100.0, py / 100.0, pz / 100.0);

	// scale
	double sx = mModelScaleXText->text().toDouble(), sy = mModelScaleYText->text().toDouble(), sz = mModelScaleZText->text().toDouble();
	model->scaled(sx, sy, sz);

	// double & vec4
	auto doubleProperties = model->doubleProperties[mShaderIdx];
	auto vecProperties = model->vecProperties[mShaderIdx];

	int baseIdx = mTexProperties[mShaderIdx].size();
	for (int i = baseIdx; i < mVPropertiesLayout->count(); i++) {
		QLayout* layout = mVPropertiesLayout->itemAt(i)->layout();
		if (i - baseIdx < doubleProperties.size()) {
			QWidget* item = layout->itemAt(1)->widget();
			QLineEdit* text = qobject_cast<QLineEdit*>(item);
			*(doubleProperties[i - baseIdx]) = text->text().toDouble();
		}
		else {
			for (int j = 0; j < 4; j++)
			{
				QWidget* item = layout->itemAt(j + 1)->widget();
				QLineEdit* text = qobject_cast<QLineEdit*>(item);
				(*(vecProperties[i - baseIdx - doubleProperties.size()]))[j] = text->text().toDouble();
			}
		}
	}
}

// ѡ��Texture Properties
void rendererApp::onPropertiesTextureButtonClicked(const int& i) {
	std::string file = readImgFile();
	if (file.empty()) return;
	auto* texture = mModels[mModelIdx]->texProperties[mShaderIdx][i];
	texture->loadTexture(file);

	QLayout* layout = mVPropertiesLayout->itemAt(i)->layout();
	QWidget* item = layout->itemAt(1)->widget();
	QLabel* canvas = qobject_cast<QLabel*>(item);
	QImage img = texture->img();
	canvas->setPixmap(QPixmap::fromImage(img).scaled(canvas->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// ���Shader
void rendererApp::onPropertiesTextureClearButtonClicked(const int& i) {
	auto* texture = mModels[mModelIdx]->texProperties[mShaderIdx][i];
	*texture = Texture(texture->isNormal());

	QLayout* layout = mVPropertiesLayout->itemAt(i)->layout();
	QWidget* item = layout->itemAt(1)->widget();
	QLabel* canvas = qobject_cast<QLabel*>(item);
	QImage img = texture->img();
	canvas->setPixmap(QPixmap::fromImage(img).scaled(canvas->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// ����ģ��
void rendererApp::onNewModelActionTriggered() {
	std::string file = readObjFile();
	QString name = QFileInfo(file.c_str()).completeBaseName();
	if (file.empty()) return;
	mModels.push_back(new Model(file));

	// Ϊ����model��properties����ʼֵ
	auto model = mModels.back();
	for (int i = 0; i < SHADERMANAGER::number; i++) {
		for (auto t : mDoubleProperties[i]) {
			model->doubleProperties[i].push_back(new double(*t));
		}
		for (auto t : mVecProperties[i]) {
			model->vecProperties[i].push_back(new vec4(*t));
		}
		for (auto t : mTexProperties[i]) {
			model->texProperties[i].push_back(new Texture(*t));
		}
	}

	// ���Comboxѡ��
	mModelIdx = mModelNum++;
	mModelBox->addItem(name);
	if (mModelNum == 1)
		mModelBox->removeItem(0);
	mModelBox->setCurrentIndex(mModelIdx);
	bindPropertiesUI();

	// ���ɾ��Action
	QAction* deleteAction = new QAction(name);
	QSignalMapper* mapper = new QSignalMapper(this);
	connect(deleteAction, SIGNAL(triggered()), mapper, SLOT(map()));
	mapper->setMapping(deleteAction, mModelIdx);
	connect(mapper, SIGNAL(mappedInt(int)), this, SLOT(onDeleteActionTriggerd(int)));
	mDeleteModelMenu->addAction(deleteAction);
}

// �л�Shader
void rendererApp::onShaderActionTriggered(const int& i) {
	mShaderIdx = i;
	bindPropertiesUI();
}

// �л�ģ��
void rendererApp::onModelBoxCurrentChanged(const int& i) {
	mModelIdx = i;
	bindPropertiesUI();
}

void rendererApp::onZTestCheckedClicked(bool checked) {
	mModels[mModelIdx]->settings.zTest = checked;
}

void rendererApp::onZWriteCheckedClicked(bool checked) {
	mModels[mModelIdx]->settings.zWrite = checked;
}

void rendererApp::onOverrideCheckClicked(bool checked) {
	mModels[mModelIdx]->settings.override = checked;
}

void rendererApp::onCullBoxCurrentChanged(const int& i) {
	mModels[mModelIdx]->settings.cull = i;
}

// ɾ��ģ��
void rendererApp::onDeleteActionTriggerd(const int& i) {
	mModelNum--;
	if (mModelNum == 0) mModelIdx = -1;

	delete mModels[i];
	mModels[i] = nullptr;

	mModels.erase(mModels.begin() + i);

	// ɾ��action
	mDeleteModelMenu->removeAction(mDeleteModelMenu->actions()[i]);

	// ɾ��combox
	if (mModelNum == 0) mModelBox->setItemText(0, "NO MODEL");
	else mModelBox->removeItem(i);
}

void rendererApp::setOrder(std::initializer_list<QWidget*> list) {
	auto last = list.begin();
	for (auto it = list.begin() + 1; it != list.end(); it++, last++) {
		setTabOrder(*last, *it);
	}
}

std::string rendererApp::readImgFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), mLastPath, tr("Images (*.png *.xpm *.jpg *.bmp)"));
	mLastPath = QFileInfo(fileName).path();
	return fileName.toStdString();
}

std::string rendererApp::readObjFile() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Model"), mLastPath, tr("OBJ Files (*.obj)"));
	mLastPath = QFileInfo(fileName).path();
	return fileName.toStdString();
}