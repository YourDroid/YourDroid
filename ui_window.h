/********************************************************************************
** Form generated from reading UI file 'window.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
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
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QFormLayout *formLayout;
    QLabel *labelArch;
    QComboBox *arch;
    QLabel *labelTBios;
    QComboBox *typeBios;
    QLabel *labelWinVer;
    QComboBox *winVer;
    QSpacerItem *verticalSpacer_6;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_15;
    QLabel *labelLanguageSettings;
    QComboBox *comboLanguageSettings;
    QCheckBox *checkEnableConSettings;
    QSpacerItem *verticalSpacer_5;
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
    QLabel *labelAboutBootloader_2;
    QLabel *labelAboutBootloader;
    QGroupBox *groupFrom;
    QGridLayout *gridLayout_5;
    QRadioButton *radioDownload;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelVersionDown;
    QComboBox *comboVersionDown;
    QRadioButton *radioChooseFromDisk;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *editImageFromDisk;
    QPushButton *buttonChooseImage;
    QGroupBox *groupWhere;
    QGridLayout *gridLayout_2;
    QRadioButton *radioInstallOnPart;
    QRadioButton *radioInstallOnDir;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *editDirForInstall;
    QPushButton *buttonChooseDirForInstall;
    QLabel *labelinfoPartIns;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *returnInstallButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonInstallInstall;
    QProgressBar *progressInstall;
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
    QPushButton *buttonDeleteDelete;
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
        Window->setWindowTitle(QStringLiteral("MainWindow"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/yourdroid.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Window->setWindowIcon(icon);
        centralwidget = new QWidget(Window);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        labelVersion = new QLabel(centralwidget);
        labelVersion->setObjectName(QStringLiteral("labelVersion"));
        labelVersion->setText(QStringLiteral("<html><head/><body><p><span style=\" font-weight:600;\">no text</span></p></body></html>"));

        horizontalLayout_2->addWidget(labelVersion);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        settingsMini = new QPushButton(centralwidget);
        settingsMini->setObjectName(QStringLiteral("settingsMini"));

        horizontalLayout_2->addWidget(settingsMini);

        labelSetInfo = new QLabel(centralwidget);
        labelSetInfo->setObjectName(QStringLiteral("labelSetInfo"));
        labelSetInfo->setText(QStringLiteral("no text"));

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
        verticalLayout_4 = new QVBoxLayout(settingsPage);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        groupBox_3 = new QGroupBox(settingsPage);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        labelArch = new QLabel(groupBox_3);
        labelArch->setObjectName(QStringLiteral("labelArch"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelArch);

        arch = new QComboBox(groupBox_3);
        arch->insertItems(0, QStringList()
         << QStringLiteral("x86")
         << QStringLiteral("x64")
        );
        arch->setObjectName(QStringLiteral("arch"));
        arch->setCurrentText(QStringLiteral("x86"));

        formLayout->setWidget(0, QFormLayout::FieldRole, arch);

        labelTBios = new QLabel(groupBox_3);
        labelTBios->setObjectName(QStringLiteral("labelTBios"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelTBios);

        typeBios = new QComboBox(groupBox_3);
        typeBios->insertItems(0, QStringList()
         << QStringLiteral("Bios")
         << QStringLiteral("Uefi")
        );
        typeBios->setObjectName(QStringLiteral("typeBios"));
        typeBios->setCurrentText(QStringLiteral("Bios"));

        formLayout->setWidget(1, QFormLayout::FieldRole, typeBios);

        labelWinVer = new QLabel(groupBox_3);
        labelWinVer->setObjectName(QStringLiteral("labelWinVer"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelWinVer);

        winVer = new QComboBox(groupBox_3);
        winVer->insertItems(0, QStringList()
         << QStringLiteral("Xp")
         << QStringLiteral("Vista+")
        );
        winVer->setObjectName(QStringLiteral("winVer"));
        winVer->setCurrentText(QStringLiteral("Xp"));

        formLayout->setWidget(2, QFormLayout::FieldRole, winVer);


        verticalLayout_6->addLayout(formLayout);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_6);


        horizontalLayout_7->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(settingsPage);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        verticalLayout_5 = new QVBoxLayout(groupBox_4);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        labelLanguageSettings = new QLabel(groupBox_4);
        labelLanguageSettings->setObjectName(QStringLiteral("labelLanguageSettings"));
        labelLanguageSettings->setText(QString::fromUtf8("Language/\320\257\320\267\321\213\320\272"));

        horizontalLayout_15->addWidget(labelLanguageSettings);

        comboLanguageSettings = new QComboBox(groupBox_4);
        comboLanguageSettings->insertItems(0, QStringList()
         << QStringLiteral("English")
         << QString::fromUtf8("\320\240\321\203\321\201\321\201\320\272\320\270\320\271")
        );
        comboLanguageSettings->setObjectName(QStringLiteral("comboLanguageSettings"));
        comboLanguageSettings->setCurrentText(QStringLiteral("English"));

        horizontalLayout_15->addWidget(comboLanguageSettings);


        verticalLayout_5->addLayout(horizontalLayout_15);

        checkEnableConSettings = new QCheckBox(groupBox_4);
        checkEnableConSettings->setObjectName(QStringLiteral("checkEnableConSettings"));

        verticalLayout_5->addWidget(checkEnableConSettings);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_5);

        checkEnableConSettings->raise();

        horizontalLayout_7->addWidget(groupBox_4);


        verticalLayout_4->addLayout(horizontalLayout_7);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

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


        verticalLayout_4->addLayout(horizontalLayout_16);

        windows->addWidget(settingsPage);
        installPage = new QWidget();
        installPage->setObjectName(QStringLiteral("installPage"));
        gridLayout_6 = new QGridLayout(installPage);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
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

        labelAboutBootloader_2 = new QLabel(groupBox_2);
        labelAboutBootloader_2->setObjectName(QStringLiteral("labelAboutBootloader_2"));
        labelAboutBootloader_2->setText(QStringLiteral("no text"));

        verticalLayout_3->addWidget(labelAboutBootloader_2);

        labelAboutBootloader = new QLabel(groupBox_2);
        labelAboutBootloader->setObjectName(QStringLiteral("labelAboutBootloader"));
        labelAboutBootloader->setText(QStringLiteral("no text"));

        verticalLayout_3->addWidget(labelAboutBootloader);


        gridLayout_4->addWidget(groupBox_2, 2, 1, 1, 1);

        groupFrom = new QGroupBox(installPage);
        groupFrom->setObjectName(QStringLiteral("groupFrom"));
        gridLayout_5 = new QGridLayout(groupFrom);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        radioDownload = new QRadioButton(groupFrom);
        radioDownload->setObjectName(QStringLiteral("radioDownload"));

        gridLayout_5->addWidget(radioDownload, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        labelVersionDown = new QLabel(groupFrom);
        labelVersionDown->setObjectName(QStringLiteral("labelVersionDown"));

        horizontalLayout_4->addWidget(labelVersionDown);

        comboVersionDown = new QComboBox(groupFrom);
        comboVersionDown->setObjectName(QStringLiteral("comboVersionDown"));

        horizontalLayout_4->addWidget(comboVersionDown);


        gridLayout_5->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        radioChooseFromDisk = new QRadioButton(groupFrom);
        radioChooseFromDisk->setObjectName(QStringLiteral("radioChooseFromDisk"));

        gridLayout_5->addWidget(radioChooseFromDisk, 2, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        editImageFromDisk = new QLineEdit(groupFrom);
        editImageFromDisk->setObjectName(QStringLiteral("editImageFromDisk"));

        horizontalLayout_5->addWidget(editImageFromDisk);

        buttonChooseImage = new QPushButton(groupFrom);
        buttonChooseImage->setObjectName(QStringLiteral("buttonChooseImage"));
        buttonChooseImage->setText(QStringLiteral("..."));

        horizontalLayout_5->addWidget(buttonChooseImage);


        gridLayout_5->addLayout(horizontalLayout_5, 3, 0, 1, 1);


        gridLayout_4->addWidget(groupFrom, 0, 0, 2, 1);

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
        buttonChooseDirForInstall->setText(QStringLiteral("..."));

        horizontalLayout_6->addWidget(buttonChooseDirForInstall);


        gridLayout_2->addLayout(horizontalLayout_6, 3, 0, 1, 1);

        labelinfoPartIns = new QLabel(groupWhere);
        labelinfoPartIns->setObjectName(QStringLiteral("labelinfoPartIns"));

        gridLayout_2->addWidget(labelinfoPartIns, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupWhere, 0, 1, 2, 1);


        gridLayout_6->addLayout(gridLayout_4, 0, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        returnInstallButton = new QPushButton(installPage);
        returnInstallButton->setObjectName(QStringLiteral("returnInstallButton"));

        horizontalLayout_8->addWidget(returnInstallButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        buttonInstallInstall = new QPushButton(installPage);
        buttonInstallInstall->setObjectName(QStringLiteral("buttonInstallInstall"));

        horizontalLayout_8->addWidget(buttonInstallInstall);


        gridLayout_6->addLayout(horizontalLayout_8, 3, 0, 1, 1);

        progressInstall = new QProgressBar(installPage);
        progressInstall->setObjectName(QStringLiteral("progressInstall"));
        progressInstall->setValue(24);

        gridLayout_6->addWidget(progressInstall, 2, 0, 1, 1);

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

        buttonDeleteDelete = new QPushButton(daletePage);
        buttonDeleteDelete->setObjectName(QStringLiteral("buttonDeleteDelete"));

        horizontalLayout_10->addWidget(buttonDeleteDelete);


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

        gridLayout->addWidget(windows, 1, 0, 1, 1);

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
        settingsMini->setText(QApplication::translate("Window", "Settings", Q_NULLPTR));
        deleteButtonMain->setText(QApplication::translate("Window", "Delete", Q_NULLPTR));
        installButtonMain->setText(QApplication::translate("Window", "Install", Q_NULLPTR));
        buttonAboutMain->setText(QApplication::translate("Window", "About", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("Window", "Features of pc", Q_NULLPTR));
        labelArch->setText(QApplication::translate("Window", "Architecture ", Q_NULLPTR));
        labelTBios->setText(QApplication::translate("Window", "Type of bios", Q_NULLPTR));
        labelWinVer->setText(QApplication::translate("Window", "Windows version", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("Window", "Settings of the app", Q_NULLPTR));
        checkEnableConSettings->setText(QApplication::translate("Window", "Output debugging console", Q_NULLPTR));
        back_settings->setText(QApplication::translate("Window", "Return", Q_NULLPTR));
        restoreSettings->setText(QApplication::translate("Window", "Restore defaults", Q_NULLPTR));
        applaySettings->setText(QApplication::translate("Window", "Apply", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Window", "Space for data", Q_NULLPTR));
        labelSizeDataInstall->setText(QApplication::translate("Window", "Size of data.img(mb):", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("Window", "Data", Q_NULLPTR));
        labelName->setText(QApplication::translate("Window", "Title:", Q_NULLPTR));
        labelBoot->setText(QApplication::translate("Window", "Bootloader:", Q_NULLPTR));
        groupFrom->setTitle(QApplication::translate("Window", "Input", Q_NULLPTR));
        radioDownload->setText(QApplication::translate("Window", "Download &ISO image", Q_NULLPTR));
        labelVersionDown->setText(QApplication::translate("Window", "Version:", Q_NULLPTR));
        radioChooseFromDisk->setText(QApplication::translate("Window", "Choose I&SO image from HD", Q_NULLPTR));
        groupWhere->setTitle(QApplication::translate("Window", "Output", Q_NULLPTR));
        radioInstallOnPart->setText(QApplication::translate("Window", "Install to separate &partition", Q_NULLPTR));
        radioInstallOnDir->setText(QApplication::translate("Window", "Install to directory", Q_NULLPTR));
        labelinfoPartIns->setText(QApplication::translate("Window", "Coming soon", Q_NULLPTR));
        returnInstallButton->setText(QApplication::translate("Window", "To main menu", Q_NULLPTR));
        buttonInstallInstall->setText(QApplication::translate("Window", "Install", Q_NULLPTR));
        labelSystemtoDelete->setText(QApplication::translate("Window", "System:", Q_NULLPTR));
        labelPlaceDelete->setText(QApplication::translate("Window", "Path:", Q_NULLPTR));
        labelPlaceDeleteText->setText(QString());
        labelBootloaderDelete->setText(QApplication::translate("Window", "Bootloader:", Q_NULLPTR));
        labelbootloaderDeleteText->setText(QString());
        buttonReturnMainDelete->setText(QApplication::translate("Window", "To main menu", Q_NULLPTR));
        buttonDeleteDelete->setText(QApplication::translate("Window", "Delete", Q_NULLPTR));
        textBrowser->setHtml(QApplication::translate("Window", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt; font-style:italic;\">The open-source program installing android to pc(android x86)</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:11pt; font-style:italic;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
                        " margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt;\">Authors:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt; font-style:italic;\">Profi_GMan</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:11pt; font-style:italic;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt;\">Speciality thanks:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt; font-style:italic;\">Atchan, severagent007</span></p></body></html>", Q_NULLPTR));
        buttonAboutReturn->setText(QApplication::translate("Window", "Return to main menu", Q_NULLPTR));
        Q_UNUSED(Window);
    } // retranslateUi

};

namespace Ui {
    class Window: public Ui_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_H
