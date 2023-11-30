/********************************************************************************
** Form generated from reading UI file 'rendererApp.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERERAPP_H
#define UI_RENDERERAPP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionRemove;
    QAction *action_newmodel;
    QAction *action_deleteModel;
    QWidget *centralwidget;
    QGroupBox *groupBox_properties;
    QScrollArea *scrollArea_properties;
    QWidget *scrollAreaWidgetContents_properties;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_properties;
    QPushButton *pushButton_properties;
    QComboBox *comboBox_model;
    QLabel *label_position;
    QLabel *label_scale;
    QLineEdit *lineEdit_modelPos_x;
    QLineEdit *lineEdit_modelPos_y;
    QLineEdit *lineEdit_modelPos_z;
    QLineEdit *lineEdit_modelScale_x;
    QLineEdit *lineEdit_modelScale_y;
    QLineEdit *lineEdit_modelScale_z;
    QLabel *label_rotation;
    QLineEdit *lineEdit_modelRotation_x;
    QLineEdit *lineEdit_modelRotation_y;
    QLineEdit *lineEdit_modelRotation_z;
    QCheckBox *checkBox_zTest;
    QCheckBox *checkBox_zWrite;
    QCheckBox *checkBox_override;
    QComboBox *comboBox_cull;
    QLabel *label_canvas;
    QGroupBox *groupBox_light;
    QLabel *label_light_rgb;
    QLineEdit *lineEdit_light_r;
    QLineEdit *lineEdit_light_g;
    QLineEdit *lineEdit_light_b;
    QPushButton *pushButton_light;
    QLabel *label_light_intensity;
    QSlider *slider_light_intensity;
    QLabel *label_light_intensity2;
    QLabel *label_light_rgb_2;
    QLineEdit *lineEdit_light_x;
    QLineEdit *lineEdit_light_y;
    QLineEdit *lineEdit_light_z;
    QLineEdit *lineEdit_ambient_r;
    QLineEdit *lineEdit_ambient_g;
    QLineEdit *lineEdit_ambient_b;
    QLabel *label_light_ambient;
    QMenuBar *menubar;
    QMenu *menu_model;
    QMenu *menu_shader;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1243, 850);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName("actionNew");
        actionRemove = new QAction(MainWindow);
        actionRemove->setObjectName("actionRemove");
        action_newmodel = new QAction(MainWindow);
        action_newmodel->setObjectName("action_newmodel");
        action_deleteModel = new QAction(MainWindow);
        action_deleteModel->setObjectName("action_deleteModel");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox_properties = new QGroupBox(centralwidget);
        groupBox_properties->setObjectName("groupBox_properties");
        groupBox_properties->setGeometry(QRect(800, 260, 441, 511));
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        font.setUnderline(true);
        groupBox_properties->setFont(font);
        scrollArea_properties = new QScrollArea(groupBox_properties);
        scrollArea_properties->setObjectName("scrollArea_properties");
        scrollArea_properties->setGeometry(QRect(0, 130, 431, 381));
        scrollArea_properties->setAcceptDrops(false);
        scrollArea_properties->setWidgetResizable(true);
        scrollAreaWidgetContents_properties = new QWidget();
        scrollAreaWidgetContents_properties->setObjectName("scrollAreaWidgetContents_properties");
        scrollAreaWidgetContents_properties->setGeometry(QRect(0, 0, 429, 379));
        verticalLayoutWidget = new QWidget(scrollAreaWidgetContents_properties);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 10, 411, 351));
        verticalLayout_properties = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_properties->setObjectName("verticalLayout_properties");
        verticalLayout_properties->setContentsMargins(10, 4, 10, 0);
        scrollArea_properties->setWidget(scrollAreaWidgetContents_properties);
        pushButton_properties = new QPushButton(groupBox_properties);
        pushButton_properties->setObjectName("pushButton_properties");
        pushButton_properties->setGeometry(QRect(350, 10, 75, 24));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setStrikeOut(false);
        pushButton_properties->setFont(font1);
        pushButton_properties->setFocusPolicy(Qt::NoFocus);
        comboBox_model = new QComboBox(groupBox_properties);
        comboBox_model->addItem(QString());
        comboBox_model->setObjectName("comboBox_model");
        comboBox_model->setGeometry(QRect(130, 5, 201, 31));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setUnderline(false);
        font2.setKerning(true);
        comboBox_model->setFont(font2);
        comboBox_model->setFocusPolicy(Qt::NoFocus);
        label_position = new QLabel(groupBox_properties);
        label_position->setObjectName("label_position");
        label_position->setGeometry(QRect(10, 50, 61, 16));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setUnderline(false);
        label_position->setFont(font3);
        label_scale = new QLabel(groupBox_properties);
        label_scale->setObjectName("label_scale");
        label_scale->setGeometry(QRect(20, 80, 31, 16));
        label_scale->setFont(font3);
        lineEdit_modelPos_x = new QLineEdit(groupBox_properties);
        lineEdit_modelPos_x->setObjectName("lineEdit_modelPos_x");
        lineEdit_modelPos_x->setGeometry(QRect(80, 50, 50, 20));
        QFont font4;
        font4.setPointSize(14);
        font4.setBold(false);
        font4.setItalic(false);
        font4.setUnderline(true);
        font4.setStrikeOut(false);
        lineEdit_modelPos_x->setFont(font4);
        lineEdit_modelPos_y = new QLineEdit(groupBox_properties);
        lineEdit_modelPos_y->setObjectName("lineEdit_modelPos_y");
        lineEdit_modelPos_y->setGeometry(QRect(150, 50, 50, 20));
        lineEdit_modelPos_y->setFont(font4);
        lineEdit_modelPos_z = new QLineEdit(groupBox_properties);
        lineEdit_modelPos_z->setObjectName("lineEdit_modelPos_z");
        lineEdit_modelPos_z->setGeometry(QRect(220, 50, 50, 20));
        lineEdit_modelPos_z->setFont(font4);
        lineEdit_modelScale_x = new QLineEdit(groupBox_properties);
        lineEdit_modelScale_x->setObjectName("lineEdit_modelScale_x");
        lineEdit_modelScale_x->setGeometry(QRect(80, 80, 50, 20));
        lineEdit_modelScale_x->setFont(font4);
        lineEdit_modelScale_y = new QLineEdit(groupBox_properties);
        lineEdit_modelScale_y->setObjectName("lineEdit_modelScale_y");
        lineEdit_modelScale_y->setGeometry(QRect(150, 80, 50, 20));
        lineEdit_modelScale_y->setFont(font4);
        lineEdit_modelScale_z = new QLineEdit(groupBox_properties);
        lineEdit_modelScale_z->setObjectName("lineEdit_modelScale_z");
        lineEdit_modelScale_z->setGeometry(QRect(220, 80, 50, 20));
        lineEdit_modelScale_z->setFont(font4);
        label_rotation = new QLabel(groupBox_properties);
        label_rotation->setObjectName("label_rotation");
        label_rotation->setGeometry(QRect(10, 110, 61, 20));
        label_rotation->setFont(font3);
        lineEdit_modelRotation_x = new QLineEdit(groupBox_properties);
        lineEdit_modelRotation_x->setObjectName("lineEdit_modelRotation_x");
        lineEdit_modelRotation_x->setGeometry(QRect(80, 110, 50, 20));
        lineEdit_modelRotation_x->setFont(font4);
        lineEdit_modelRotation_y = new QLineEdit(groupBox_properties);
        lineEdit_modelRotation_y->setObjectName("lineEdit_modelRotation_y");
        lineEdit_modelRotation_y->setGeometry(QRect(150, 110, 50, 20));
        lineEdit_modelRotation_y->setFont(font4);
        lineEdit_modelRotation_z = new QLineEdit(groupBox_properties);
        lineEdit_modelRotation_z->setObjectName("lineEdit_modelRotation_z");
        lineEdit_modelRotation_z->setGeometry(QRect(220, 110, 50, 20));
        lineEdit_modelRotation_z->setFont(font4);
        checkBox_zTest = new QCheckBox(groupBox_properties);
        checkBox_zTest->setObjectName("checkBox_zTest");
        checkBox_zTest->setGeometry(QRect(280, 50, 79, 20));
        checkBox_zTest->setFont(font3);
        checkBox_zTest->setChecked(true);
        checkBox_zWrite = new QCheckBox(groupBox_properties);
        checkBox_zWrite->setObjectName("checkBox_zWrite");
        checkBox_zWrite->setGeometry(QRect(280, 100, 79, 20));
        checkBox_zWrite->setFont(font3);
        checkBox_zWrite->setChecked(true);
        checkBox_override = new QCheckBox(groupBox_properties);
        checkBox_override->setObjectName("checkBox_override");
        checkBox_override->setGeometry(QRect(350, 50, 71, 20));
        checkBox_override->setFont(font3);
        comboBox_cull = new QComboBox(groupBox_properties);
        comboBox_cull->addItem(QString());
        comboBox_cull->addItem(QString());
        comboBox_cull->addItem(QString());
        comboBox_cull->setObjectName("comboBox_cull");
        comboBox_cull->setEnabled(true);
        comboBox_cull->setGeometry(QRect(350, 100, 68, 22));
        comboBox_cull->setFont(font3);
        comboBox_cull->setFocusPolicy(Qt::NoFocus);
        label_canvas = new QLabel(centralwidget);
        label_canvas->setObjectName("label_canvas");
        label_canvas->setGeometry(QRect(0, 0, 800, 800));
        label_canvas->setMouseTracking(true);
        groupBox_light = new QGroupBox(centralwidget);
        groupBox_light->setObjectName("groupBox_light");
        groupBox_light->setGeometry(QRect(800, 20, 391, 241));
        QFont font5;
        font5.setPointSize(15);
        font5.setBold(true);
        font5.setItalic(false);
        font5.setUnderline(true);
        font5.setStrikeOut(false);
        groupBox_light->setFont(font5);
        label_light_rgb = new QLabel(groupBox_light);
        label_light_rgb->setObjectName("label_light_rgb");
        label_light_rgb->setGeometry(QRect(20, 44, 41, 31));
        QFont font6;
        font6.setPointSize(13);
        font6.setBold(false);
        font6.setItalic(false);
        font6.setUnderline(false);
        font6.setStrikeOut(false);
        label_light_rgb->setFont(font6);
        lineEdit_light_r = new QLineEdit(groupBox_light);
        lineEdit_light_r->setObjectName("lineEdit_light_r");
        lineEdit_light_r->setGeometry(QRect(60, 50, 50, 20));
        lineEdit_light_r->setFont(font4);
        lineEdit_light_g = new QLineEdit(groupBox_light);
        lineEdit_light_g->setObjectName("lineEdit_light_g");
        lineEdit_light_g->setGeometry(QRect(130, 50, 50, 20));
        QFont font7;
        font7.setPointSize(15);
        font7.setBold(false);
        font7.setItalic(false);
        font7.setUnderline(true);
        font7.setStrikeOut(false);
        lineEdit_light_g->setFont(font7);
        lineEdit_light_b = new QLineEdit(groupBox_light);
        lineEdit_light_b->setObjectName("lineEdit_light_b");
        lineEdit_light_b->setGeometry(QRect(200, 50, 50, 20));
        lineEdit_light_b->setFont(font7);
        pushButton_light = new QPushButton(groupBox_light);
        pushButton_light->setObjectName("pushButton_light");
        pushButton_light->setGeometry(QRect(120, 200, 75, 24));
        pushButton_light->setFont(font1);
        pushButton_light->setFocusPolicy(Qt::NoFocus);
        label_light_intensity = new QLabel(groupBox_light);
        label_light_intensity->setObjectName("label_light_intensity");
        label_light_intensity->setGeometry(QRect(20, 80, 41, 31));
        QFont font8;
        font8.setPointSize(9);
        font8.setBold(false);
        font8.setItalic(false);
        font8.setUnderline(false);
        font8.setStrikeOut(false);
        label_light_intensity->setFont(font8);
        slider_light_intensity = new QSlider(groupBox_light);
        slider_light_intensity->setObjectName("slider_light_intensity");
        slider_light_intensity->setGeometry(QRect(60, 90, 131, 16));
        slider_light_intensity->setCursor(QCursor(Qt::OpenHandCursor));
        slider_light_intensity->setMaximum(30);
        slider_light_intensity->setSingleStep(1);
        slider_light_intensity->setValue(10);
        slider_light_intensity->setOrientation(Qt::Horizontal);
        label_light_intensity2 = new QLabel(groupBox_light);
        label_light_intensity2->setObjectName("label_light_intensity2");
        label_light_intensity2->setGeometry(QRect(210, 83, 41, 31));
        label_light_intensity2->setFont(font6);
        label_light_rgb_2 = new QLabel(groupBox_light);
        label_light_rgb_2->setObjectName("label_light_rgb_2");
        label_light_rgb_2->setGeometry(QRect(20, 113, 41, 31));
        label_light_rgb_2->setFont(font6);
        lineEdit_light_x = new QLineEdit(groupBox_light);
        lineEdit_light_x->setObjectName("lineEdit_light_x");
        lineEdit_light_x->setGeometry(QRect(60, 120, 50, 20));
        QFont font9;
        font9.setPointSize(10);
        font9.setBold(false);
        font9.setItalic(false);
        font9.setUnderline(false);
        font9.setStrikeOut(false);
        lineEdit_light_x->setFont(font9);
        lineEdit_light_y = new QLineEdit(groupBox_light);
        lineEdit_light_y->setObjectName("lineEdit_light_y");
        lineEdit_light_y->setGeometry(QRect(130, 120, 50, 20));
        QFont font10;
        font10.setPointSize(8);
        font10.setBold(false);
        font10.setItalic(false);
        font10.setUnderline(false);
        font10.setStrikeOut(false);
        lineEdit_light_y->setFont(font10);
        lineEdit_light_z = new QLineEdit(groupBox_light);
        lineEdit_light_z->setObjectName("lineEdit_light_z");
        lineEdit_light_z->setGeometry(QRect(200, 120, 50, 20));
        lineEdit_light_z->setFont(font9);
        lineEdit_ambient_r = new QLineEdit(groupBox_light);
        lineEdit_ambient_r->setObjectName("lineEdit_ambient_r");
        lineEdit_ambient_r->setGeometry(QRect(60, 160, 50, 20));
        lineEdit_ambient_r->setFont(font4);
        lineEdit_ambient_g = new QLineEdit(groupBox_light);
        lineEdit_ambient_g->setObjectName("lineEdit_ambient_g");
        lineEdit_ambient_g->setGeometry(QRect(130, 160, 50, 20));
        lineEdit_ambient_g->setFont(font7);
        lineEdit_ambient_b = new QLineEdit(groupBox_light);
        lineEdit_ambient_b->setObjectName("lineEdit_ambient_b");
        lineEdit_ambient_b->setGeometry(QRect(200, 160, 50, 20));
        lineEdit_ambient_b->setFont(font7);
        label_light_ambient = new QLabel(groupBox_light);
        label_light_ambient->setObjectName("label_light_ambient");
        label_light_ambient->setGeometry(QRect(10, 155, 51, 31));
        label_light_ambient->setFont(font8);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1243, 22));
        menu_model = new QMenu(menubar);
        menu_model->setObjectName("menu_model");
        menu_shader = new QMenu(menubar);
        menu_shader->setObjectName("menu_shader");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(lineEdit_light_r, lineEdit_light_g);
        QWidget::setTabOrder(lineEdit_light_g, lineEdit_light_b);
        QWidget::setTabOrder(lineEdit_light_b, slider_light_intensity);
        QWidget::setTabOrder(slider_light_intensity, lineEdit_light_x);
        QWidget::setTabOrder(lineEdit_light_x, lineEdit_light_y);
        QWidget::setTabOrder(lineEdit_light_y, lineEdit_light_z);
        QWidget::setTabOrder(lineEdit_light_z, lineEdit_ambient_r);
        QWidget::setTabOrder(lineEdit_ambient_r, lineEdit_ambient_g);
        QWidget::setTabOrder(lineEdit_ambient_g, lineEdit_ambient_b);
        QWidget::setTabOrder(lineEdit_ambient_b, lineEdit_modelPos_x);
        QWidget::setTabOrder(lineEdit_modelPos_x, lineEdit_modelPos_y);
        QWidget::setTabOrder(lineEdit_modelPos_y, lineEdit_modelPos_z);
        QWidget::setTabOrder(lineEdit_modelPos_z, lineEdit_modelScale_x);
        QWidget::setTabOrder(lineEdit_modelScale_x, lineEdit_modelScale_y);
        QWidget::setTabOrder(lineEdit_modelScale_y, lineEdit_modelScale_z);
        QWidget::setTabOrder(lineEdit_modelScale_z, lineEdit_modelRotation_x);
        QWidget::setTabOrder(lineEdit_modelRotation_x, lineEdit_modelRotation_y);
        QWidget::setTabOrder(lineEdit_modelRotation_y, lineEdit_modelRotation_z);
        QWidget::setTabOrder(lineEdit_modelRotation_z, checkBox_zTest);
        QWidget::setTabOrder(checkBox_zTest, checkBox_zWrite);
        QWidget::setTabOrder(checkBox_zWrite, checkBox_override);
        QWidget::setTabOrder(checkBox_override, scrollArea_properties);
        QWidget::setTabOrder(scrollArea_properties, comboBox_cull);
        QWidget::setTabOrder(comboBox_cull, comboBox_model);

        menubar->addAction(menu_model->menuAction());
        menubar->addAction(menu_shader->menuAction());
        menu_model->addSeparator();
        menu_model->addAction(action_newmodel);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "New...", nullptr));
        actionRemove->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        action_newmodel->setText(QCoreApplication::translate("MainWindow", "New...", nullptr));
        action_deleteModel->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        groupBox_properties->setTitle(QCoreApplication::translate("MainWindow", "Properties", nullptr));
        pushButton_properties->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232", nullptr));
        comboBox_model->setItemText(0, QCoreApplication::translate("MainWindow", "NO MODEL", nullptr));

        label_position->setText(QCoreApplication::translate("MainWindow", "Position", nullptr));
        label_scale->setText(QCoreApplication::translate("MainWindow", "Scale", nullptr));
        label_rotation->setText(QCoreApplication::translate("MainWindow", "Rotation", nullptr));
        checkBox_zTest->setText(QCoreApplication::translate("MainWindow", "zTest", nullptr));
        checkBox_zWrite->setText(QCoreApplication::translate("MainWindow", "zWrite", nullptr));
        checkBox_override->setText(QCoreApplication::translate("MainWindow", "overrider", nullptr));
        comboBox_cull->setItemText(0, QCoreApplication::translate("MainWindow", "Back", nullptr));
        comboBox_cull->setItemText(1, QCoreApplication::translate("MainWindow", "Front", nullptr));
        comboBox_cull->setItemText(2, QCoreApplication::translate("MainWindow", "None", nullptr));

        label_canvas->setText(QString());
        groupBox_light->setTitle(QCoreApplication::translate("MainWindow", "Light", nullptr));
        label_light_rgb->setText(QCoreApplication::translate("MainWindow", "RGB", nullptr));
        pushButton_light->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232", nullptr));
        label_light_intensity->setText(QCoreApplication::translate("MainWindow", "Inten-", nullptr));
        label_light_intensity2->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        label_light_rgb_2->setText(QCoreApplication::translate("MainWindow", "Dir", nullptr));
        lineEdit_light_x->setText(QString());
        label_light_ambient->setText(QCoreApplication::translate("MainWindow", "Ambient", nullptr));
        menu_model->setTitle(QCoreApplication::translate("MainWindow", "Model", nullptr));
        menu_shader->setTitle(QCoreApplication::translate("MainWindow", "Shader", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERERAPP_H
