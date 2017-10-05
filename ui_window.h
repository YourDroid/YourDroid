/********************************************************************************
** Form generated from reading UI file 'window.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Window
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelVersion;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *settingsMini;
    QLabel *labelSetInfo;
    QStackedWidget *windows;
    QWidget *mainWindowPage;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *deleteButtonMain;
    QPushButton *installButtonMain;
    QSpacerItem *verticalSpacer_2;
    QPushButton *buttonAboutMain;
    QWidget *settingsPage;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_20;
    QFormLayout *formLayout;
    QLabel *labelArch;
    QComboBox *arch;
    QLabel *labelTBios;
    QComboBox *typeBios;
    QLabel *labelWinVer;
    QComboBox *winVer;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *back_settings;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *restoreSettings;
    QPushButton *applaySettings;
    QWidget *installPage;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_4;
    QGroupBox *groupFrom;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelVersionDown;
    QComboBox *comboVersionDown;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *editImageFromDisk;
    QPushButton *buttonChooseImage;
    QRadioButton *radioChooseFromDisk;
    QRadioButton *radioDownload;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_17;
    QLabel *labelSizeDataInstall;
    QLineEdit *editSizeDataInstall;
    QSpacerItem *verticalSpacer_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *labelName;
    QLineEdit *editName;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelBoot;
    QComboBox *comboBoot;
    QHBoxLayout *horizontalLayout_19;
    QLabel *labelAboutBootloader;
    QGroupBox *groupWhere;
    QGridLayout *gridLayout_2;
    QRadioButton *radioInstallOnPart;
    QRadioButton *radioInstallOnDir;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *editDirForInstall;
    QPushButton *buttonChooseDirForInstall;
    QLabel *labelinfoPartIns;
    QProgressBar *progressInstall;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *returnInstallButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonStopInstall;
    QPushButton *buttonInstallInstall;
    QWidget *daletePage;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_14;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_11;
    QLabel *labelSystemtoDelete;
    QComboBox *comboSystemDelete;
    QHBoxLayout *horizontalLayout_13;
    QLabel *labelPlaceDelete;
    QLabel *labelPlaceDeleteText;
    QHBoxLayout *horizontalLayout_12;
    QLabel *labelBootloaderDelete;
    QLabel *labelbootloaderDeleteText;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *verticalSpacer_3;
    QProgressBar *progressDelete;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *buttonReturnMainDelete;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pushButton_2;
    QWidget *aboutPage;
    QVBoxLayout *verticalLayout_8;
    QTextBrowser *textBrowser;
    QPushButton *buttonAboutReturn;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Window)
    {
        if (Window->objectName().isEmpty())
            Window->setObjectName(QStringLiteral("Window"));
        Window->resize(653, 485);
        Window->setMinimumSize(QSize(653, 485));
        Window->setMaximumSize(QSize(653, 485));
        centralwidget = new QWidget(Window);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        labelVersion = new QLabel(centralwidget);
        labelVersion->setObjectName(QStringLiteral("labelVersion"));

        horizontalLayout_2->addWidget(labelVersion);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        settingsMini = new QPushButton(centralwidget);
        settingsMini->setObjectName(QStringLiteral("settingsMini"));

        horizontalLayout_2->addWidget(settingsMini);

        labelSetInfo = new QLabel(centralwidget);
        labelSetInfo->setObjectName(QStringLiteral("labelSetInfo"));

        horizontalLayout_2->addWidget(labelSetInfo);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        windows = new QStackedWidget(centralwidget);
        windows->setObjectName(QStringLiteral("windows"));
        mainWindowPage = new QWidget();
        mainWindowPage->setObjectName(QStringLiteral("mainWindowPage"));
        gridLayout_3 = new QGridLayout(mainWindowPage);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        deleteButtonMain = new QPushButton(mainWindowPage);
        deleteButtonMain->setObjectName(QStringLiteral("deleteButtonMain"));

        gridLayout_3->addWidget(deleteButtonMain, 1, 0, 1, 1);

        installButtonMain = new QPushButton(mainWindowPage);
        installButtonMain->setObjectName(QStringLiteral("installButtonMain"));

        gridLayout_3->addWidget(installButtonMain, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 3, 0, 1, 1);

        buttonAboutMain = new QPushButton(mainWindowPage);
        buttonAboutMain->setObjectName(QStringLiteral("buttonAboutMain"));

        gridLayout_3->addWidget(buttonAboutMain, 2, 0, 1, 1);

        windows->addWidget(mainWindowPage);
        settingsPage = new QWidget();
        settingsPage->setObjectName(QStringLiteral("settingsPage"));
        verticalLayout_7 = new QVBoxLayout(settingsPage);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        labelArch = new QLabel(settingsPage);
        labelArch->setObjectName(QStringLiteral("labelArch"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelArch);

        arch = new QComboBox(settingsPage);
        arch->setObjectName(QStringLiteral("arch"));

        formLayout->setWidget(0, QFormLayout::FieldRole, arch);

        labelTBios = new QLabel(settingsPage);
        labelTBios->setObjectName(QStringLiteral("labelTBios"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelTBios);

        typeBios = new QComboBox(settingsPage);
        typeBios->setObjectName(QStringLiteral("typeBios"));

        formLayout->setWidget(1, QFormLayout::FieldRole, typeBios);

        labelWinVer = new QLabel(settingsPage);
        labelWinVer->setObjectName(QStringLiteral("labelWinVer"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelWinVer);

        winVer = new QComboBox(settingsPage);
        winVer->setObjectName(QStringLiteral("winVer"));

        formLayout->setWidget(2, QFormLayout::FieldRole, winVer);


        horizontalLayout_20->addLayout(formLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer);


        verticalLayout_7->addLayout(horizontalLayout_20);

        verticalSpacer = new QSpacerItem(20, 255, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        back_settings = new QPushButton(settingsPage);
        back_settings->setObjectName(QStringLiteral("back_settings"));

        horizontalLayout_16->addWidget(back_settings);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_6);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        restoreSettings = new QPushButton(settingsPage);
        restoreSettings->setObjectName(QStringLiteral("restoreSettings"));

        horizontalLayout_9->addWidget(restoreSettings);

        applaySettings = new QPushButton(settingsPage);
        applaySettings->setObjectName(QStringLiteral("applaySettings"));

        horizontalLayout_9->addWidget(applaySettings);


        horizontalLayout_16->addLayout(horizontalLayout_9);


        verticalLayout_7->addLayout(horizontalLayout_16);

        windows->addWidget(settingsPage);
        installPage = new QWidget();
        installPage->setObjectName(QStringLiteral("installPage"));
        gridLayout_6 = new QGridLayout(installPage);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        groupFrom = new QGroupBox(installPage);
        groupFrom->setObjectName(QStringLiteral("groupFrom"));
        gridLayout_5 = new QGridLayout(groupFrom);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        labelVersionDown = new QLabel(groupFrom);
        labelVersionDown->setObjectName(QStringLiteral("labelVersionDown"));

        horizontalLayout_4->addWidget(labelVersionDown);

        comboVersionDown = new QComboBox(groupFrom);
        comboVersionDown->setObjectName(QStringLiteral("comboVersionDown"));

        horizontalLayout_4->addWidget(comboVersionDown);


        gridLayout_5->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        editImageFromDisk = new QLineEdit(groupFrom);
        editImageFromDisk->setObjectName(QStringLiteral("editImageFromDisk"));

        horizontalLayout_5->addWidget(editImageFromDisk);

        buttonChooseImage = new QPushButton(groupFrom);
        buttonChooseImage->setObjectName(QStringLiteral("buttonChooseImage"));

        horizontalLayout_5->addWidget(buttonChooseImage);


        gridLayout_5->addLayout(horizontalLayout_5, 3, 0, 1, 1);

        radioChooseFromDisk = new QRadioButton(groupFrom);
        radioChooseFromDisk->setObjectName(QStringLiteral("radioChooseFromDisk"));

        gridLayout_5->addWidget(radioChooseFromDisk, 2, 0, 1, 1);

        radioDownload = new QRadioButton(groupFrom);
        radioDownload->setObjectName(QStringLiteral("radioDownload"));

        gridLayout_5->addWidget(radioDownload, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupFrom, 0, 0, 2, 1);

        groupBox = new QGroupBox(installPage);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        labelSizeDataInstall = new QLabel(groupBox);
        labelSizeDataInstall->setObjectName(QStringLiteral("labelSizeDataInstall"));

        horizontalLayout_17->addWidget(labelSizeDataInstall);

        editSizeDataInstall = new QLineEdit(groupBox);
        editSizeDataInstall->setObjectName(QStringLiteral("editSizeDataInstall"));

        horizontalLayout_17->addWidget(editSizeDataInstall);


        verticalLayout_2->addLayout(horizontalLayout_17);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);


        gridLayout_4->addWidget(groupBox, 2, 0, 1, 1);

        groupBox_2 = new QGroupBox(installPage);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        labelName = new QLabel(groupBox_2);
        labelName->setObjectName(QStringLiteral("labelName"));

        horizontalLayout->addWidget(labelName);

        editName = new QLineEdit(groupBox_2);
        editName->setObjectName(QStringLiteral("editName"));

        horizontalLayout->addWidget(editName);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        labelBoot = new QLabel(groupBox_2);
        labelBoot->setObjectName(QStringLiteral("labelBoot"));

        horizontalLayout_3->addWidget(labelBoot);

        comboBoot = new QComboBox(groupBox_2);
        comboBoot->setObjectName(QStringLiteral("comboBoot"));

        horizontalLayout_3->addWidget(comboBoot);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        labelAboutBootloader = new QLabel(groupBox_2);
        labelAboutBootloader->setObjectName(QStringLiteral("labelAboutBootloader"));

        horizontalLayout_19->addWidget(labelAboutBootloader);


        verticalLayout_3->addLayout(horizontalLayout_19);


        gridLayout_4->addWidget(groupBox_2, 2, 1, 1, 1);

        groupWhere = new QGroupBox(installPage);
        groupWhere->setObjectName(QStringLiteral("groupWhere"));
        gridLayout_2 = new QGridLayout(groupWhere);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        radioInstallOnPart = new QRadioButton(groupWhere);
        radioInstallOnPart->setObjectName(QStringLiteral("radioInstallOnPart"));

        gridLayout_2->addWidget(radioInstallOnPart, 0, 0, 1, 1);

        radioInstallOnDir = new QRadioButton(groupWhere);
        radioInstallOnDir->setObjectName(QStringLiteral("radioInstallOnDir"));

        gridLayout_2->addWidget(radioInstallOnDir, 2, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        editDirForInstall = new QLineEdit(groupWhere);
        editDirForInstall->setObjectName(QStringLiteral("editDirForInstall"));

        horizontalLayout_6->addWidget(editDirForInstall);

        buttonChooseDirForInstall = new QPushButton(groupWhere);
        buttonChooseDirForInstall->setObjectName(QStringLiteral("buttonChooseDirForInstall"));

        horizontalLayout_6->addWidget(buttonChooseDirForInstall);


        gridLayout_2->addLayout(horizontalLayout_6, 3, 0, 1, 1);

        labelinfoPartIns = new QLabel(groupWhere);
        labelinfoPartIns->setObjectName(QStringLiteral("labelinfoPartIns"));

        gridLayout_2->addWidget(labelinfoPartIns, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupWhere, 0, 1, 2, 1);


        gridLayout_6->addLayout(gridLayout_4, 0, 0, 1, 1);

        progressInstall = new QProgressBar(installPage);
        progressInstall->setObjectName(QStringLiteral("progressInstall"));
        progressInstall->setValue(24);

        gridLayout_6->addWidget(progressInstall, 1, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        returnInstallButton = new QPushButton(installPage);
        returnInstallButton->setObjectName(QStringLiteral("returnInstallButton"));

        horizontalLayout_8->addWidget(returnInstallButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        buttonStopInstall = new QPushButton(installPage);
        buttonStopInstall->setObjectName(QStringLiteral("buttonStopInstall"));

        horizontalLayout_8->addWidget(buttonStopInstall);

        buttonInstallInstall = new QPushButton(installPage);
        buttonInstallInstall->setObjectName(QStringLiteral("buttonInstallInstall"));

        horizontalLayout_8->addWidget(buttonInstallInstall);


        gridLayout_6->addLayout(horizontalLayout_8, 2, 0, 1, 1);

        windows->addWidget(installPage);
        daletePage = new QWidget();
        daletePage->setObjectName(QStringLiteral("daletePage"));
        verticalLayout_9 = new QVBoxLayout(daletePage);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        labelSystemtoDelete = new QLabel(daletePage);
        labelSystemtoDelete->setObjectName(QStringLiteral("labelSystemtoDelete"));

        horizontalLayout_11->addWidget(labelSystemtoDelete);

        comboSystemDelete = new QComboBox(daletePage);
        comboSystemDelete->setObjectName(QStringLiteral("comboSystemDelete"));

        horizontalLayout_11->addWidget(comboSystemDelete);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        labelPlaceDelete = new QLabel(daletePage);
        labelPlaceDelete->setObjectName(QStringLiteral("labelPlaceDelete"));

        horizontalLayout_13->addWidget(labelPlaceDelete);

        labelPlaceDeleteText = new QLabel(daletePage);
        labelPlaceDeleteText->setObjectName(QStringLiteral("labelPlaceDeleteText"));

        horizontalLayout_13->addWidget(labelPlaceDeleteText);


        verticalLayout->addLayout(horizontalLayout_13);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        labelBootloaderDelete = new QLabel(daletePage);
        labelBootloaderDelete->setObjectName(QStringLiteral("labelBootloaderDelete"));

        horizontalLayout_12->addWidget(labelBootloaderDelete);

        labelbootloaderDeleteText = new QLabel(daletePage);
        labelbootloaderDeleteText->setObjectName(QStringLiteral("labelbootloaderDeleteText"));

        horizontalLayout_12->addWidget(labelbootloaderDeleteText);


        verticalLayout->addLayout(horizontalLayout_12);


        horizontalLayout_14->addLayout(verticalLayout);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_11);


        verticalLayout_9->addLayout(horizontalLayout_14);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_3);

        progressDelete = new QProgressBar(daletePage);
        progressDelete->setObjectName(QStringLiteral("progressDelete"));
        progressDelete->setValue(24);

        verticalLayout_9->addWidget(progressDelete);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        buttonReturnMainDelete = new QPushButton(daletePage);
        buttonReturnMainDelete->setObjectName(QStringLiteral("buttonReturnMainDelete"));

        horizontalLayout_10->addWidget(buttonReturnMainDelete);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_10);

        pushButton_2 = new QPushButton(daletePage);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_10->addWidget(pushButton_2);


        verticalLayout_9->addLayout(horizontalLayout_10);

        windows->addWidget(daletePage);
        aboutPage = new QWidget();
        aboutPage->setObjectName(QStringLiteral("aboutPage"));
        verticalLayout_8 = new QVBoxLayout(aboutPage);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        textBrowser = new QTextBrowser(aboutPage);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout_8->addWidget(textBrowser);

        buttonAboutReturn = new QPushButton(aboutPage);
        buttonAboutReturn->setObjectName(QStringLiteral("buttonAboutReturn"));

        verticalLayout_8->addWidget(buttonAboutReturn);

        windows->addWidget(aboutPage);

        gridLayout->addWidget(windows, 2, 0, 1, 1);

        Window->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Window);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        Window->setStatusBar(statusbar);

        retranslateUi(Window);

        windows->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Window);
    } // setupUi

    void retranslateUi(QMainWindow *Window)
    {
        Window->setWindowTitle(QApplication::translate("Window", "MainWindow", Q_NULLPTR));
        labelVersion->setText(QApplication::translate("Window", "<html><head/><body><p><span style=\" font-weight:600;\">2.0.0</span></p></body></html>", Q_NULLPTR));
        settingsMini->setText(QApplication::translate("Window", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", Q_NULLPTR));
        labelSetInfo->setText(QApplication::translate("Window", "Uefi x64", Q_NULLPTR));
        deleteButtonMain->setText(QApplication::translate("Window", "\320\243\320\264\320\260\320\273\320\265\320\275\320\270\320\265", Q_NULLPTR));
        installButtonMain->setText(QApplication::translate("Window", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\260", Q_NULLPTR));
        buttonAboutMain->setText(QApplication::translate("Window", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", Q_NULLPTR));
        labelArch->setText(QApplication::translate("Window", "\320\220\321\200\321\205\320\265\321\202\320\270\320\272\321\202\321\203\321\200\320\260", Q_NULLPTR));
        arch->clear();
        arch->insertItems(0, QStringList()
         << QApplication::translate("Window", "x86", Q_NULLPTR)
         << QApplication::translate("Window", "x64", Q_NULLPTR)
        );
        labelTBios->setText(QApplication::translate("Window", "\320\242\320\270\320\277 \320\261\320\270\320\276\321\201\320\260", Q_NULLPTR));
        typeBios->clear();
        typeBios->insertItems(0, QStringList()
         << QApplication::translate("Window", "Bios", Q_NULLPTR)
         << QApplication::translate("Window", "Uefi", Q_NULLPTR)
        );
        labelWinVer->setText(QApplication::translate("Window", "\320\222\320\265\321\200\321\201\320\270\321\217 Windows", Q_NULLPTR));
        winVer->clear();
        winVer->insertItems(0, QStringList()
         << QApplication::translate("Window", "Xp", Q_NULLPTR)
         << QApplication::translate("Window", "Vista+", Q_NULLPTR)
        );
        back_settings->setText(QApplication::translate("Window", "\320\235\320\260\320\267\320\260\320\264", Q_NULLPTR));
        restoreSettings->setText(QApplication::translate("Window", "\320\237\320\276 \321\203\320\274\320\276\320\273\321\207\320\260\320\275\320\270\321\216", Q_NULLPTR));
        applaySettings->setText(QApplication::translate("Window", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", Q_NULLPTR));
        groupFrom->setTitle(QApplication::translate("Window", "\320\236\321\202\320\272\321\203\320\264\320\260 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214?", Q_NULLPTR));
        labelVersionDown->setText(QApplication::translate("Window", "\320\222\320\265\321\200\321\201\320\270\321\217:", Q_NULLPTR));
        buttonChooseImage->setText(QApplication::translate("Window", "...", Q_NULLPTR));
        radioChooseFromDisk->setText(QApplication::translate("Window", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 I&SO \320\276\320\261\321\200\320\260\320\267 \321\201 \320\264\320\270\321\201\320\272\320\260", Q_NULLPTR));
        radioDownload->setText(QApplication::translate("Window", "\320\241\320\272\320\260\321\207\320\260\321\202\321\214 &ISO \320\276\320\261\321\200\320\260\320\267", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Window", "\320\241\320\272\320\276\320\273\321\214\320\272\320\276 \320\262\321\213\320\264\320\265\320\273\320\270\321\202\321\214 \320\274\320\265\321\201\321\202\320\260 \320\277\320\276\320\264 \320\264\320\260\320\275\320\275\321\213\320\265?", Q_NULLPTR));
        labelSizeDataInstall->setText(QApplication::translate("Window", "\320\240\320\260\320\267\320\274\320\265\321\200 data.img(\320\274\320\261):", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("Window", "\320\232\320\260\320\272 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214?", Q_NULLPTR));
        labelName->setText(QApplication::translate("Window", "\320\230\320\274\321\217", Q_NULLPTR));
        labelBoot->setText(QApplication::translate("Window", "\320\227\320\260\320\263\321\200\321\203\320\267\321\207\320\270\320\272", Q_NULLPTR));
        labelAboutBootloader->setText(QString());
        groupWhere->setTitle(QApplication::translate("Window", "\320\232\321\203\320\264\320\260 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214?", Q_NULLPTR));
        radioInstallOnPart->setText(QApplication::translate("Window", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214 \320\275\320\260 &\321\200\320\260\320\267\320\264\320\265\320\273", Q_NULLPTR));
        radioInstallOnDir->setText(QApplication::translate("Window", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214 \320\262 \320\264\320\270\321\200\320\265\320\272\321\202\320\276\321\200\320\270\321\216", Q_NULLPTR));
        buttonChooseDirForInstall->setText(QApplication::translate("Window", "...", Q_NULLPTR));
        labelinfoPartIns->setText(QApplication::translate("Window", "\320\241\320\272\320\276\321\200\320\276", Q_NULLPTR));
        returnInstallButton->setText(QApplication::translate("Window", "\320\235\320\260\320\267\320\260\320\264, \320\262 \320\263\320\273\320\260\320\262\320\275\320\276\320\265 \320\274\320\265\320\275\321\216", Q_NULLPTR));
        buttonStopInstall->setText(QApplication::translate("Window", "\320\241\321\202\320\276\320\277", Q_NULLPTR));
        buttonInstallInstall->setText(QApplication::translate("Window", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214", Q_NULLPTR));
        labelSystemtoDelete->setText(QApplication::translate("Window", "\320\241\320\270\321\201\321\202\320\265\320\274\320\260:", Q_NULLPTR));
        labelPlaceDelete->setText(QApplication::translate("Window", "\320\237\321\203\321\202\321\214:", Q_NULLPTR));
        labelPlaceDeleteText->setText(QString());
        labelBootloaderDelete->setText(QApplication::translate("Window", "\320\227\320\260\320\263\321\200\321\203\320\267\321\207\320\270\320\272:", Q_NULLPTR));
        labelbootloaderDeleteText->setText(QString());
        buttonReturnMainDelete->setText(QApplication::translate("Window", "\320\235\320\260\320\267\320\260\320\264", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Window", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", Q_NULLPTR));
        textBrowser->setHtml(QApplication::translate("Window", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt;\">\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; font-style:italic;\">\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\260 \321\201 \320\276\321\202\320\272\321\200\321\213\321\202\321\213\320\274 \320\270\321\201\321\205\320\276\320\264\320\275\321\213\320\274 \320\272\320\276\320\264\320\276"
                        "\320\274 \320\264\320\273\321\217 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 \320\277\320\276\321\200\321\202\320\260 \320\260\320\275\320\264\321\200\320\276\320\270\320\264\320\260 \320\275\320\260 \320\277\320\272, android x86</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Ubuntu'; font-size:11pt; font-style:italic;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt;\">\320\220\320\262\321\202\320\276\321\200\321\213:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; font-style:italic;\">Profi_GMan</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px"
                        "; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Ubuntu'; font-size:11pt; font-style:italic;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt;\">\320\236\321\201\320\276\320\261\321\213\320\265 \320\261\320\273\320\260\320\263\320\276\320\264\320\260\321\200\320\275\320\276\321\201\321\202\320\270:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt; font-style:italic;\">Atchan</span></p></body></html>", Q_NULLPTR));
        buttonAboutReturn->setText(QApplication::translate("Window", "\320\222\320\265\321\200\320\275\321\203\321\202\321\214\321\201\321\217 \320\262 \320\274\320\265\320\275\321\216", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Window: public Ui_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_H
