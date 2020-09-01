#include "window.h"
#include "ui_window.h"
#include "3rdparty/enum.h"
#include "3rdparty/tagged_bool.h"
#include <string>
#include "install.h"
#include "cmd.h"
#include "global.h"
#include <QValidator>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QFutureWatcher>
#include <QtWidgets>

Window::Window(bool f, QWidget *parent) :
    QMainWindow(parent),
    first(!f),
    ui(new Ui::Window)
{
    if(global->set->tbios == false && OS == 0) {
        qCritical().noquote() << tr("^This PC is not supported");
    }

    //setWindowIcon(QIcon(":/yourdroid.png"));

    qDebug().noquote() << "Setting window";

    ui->setupUi(this);
    setLabelSetInfo();

#if WIN
    if (QtWin::isCompositionEnabled())
        QtWin::extendFrameIntoClientArea(this, 0, 0, 0, 0);
    else
        QtWin::resetExtendedFrame(this);
#endif

    log::consoleSetParent(this);

    connect(ui->returnInstallButton,SIGNAL(clicked()),SLOT(returnMainWindow()));
    connect(ui->settingsMini,&QPushButton::clicked,[=](){
        if(ui->windows->currentWidget() == ui->settingsPage) on_back_settings_clicked();
        else Settings_clicked();
    });
    connect(ui->buttonAboutReturn,SIGNAL(clicked()),SLOT(returnMainWindow()));
    connect(ui->windows, &QStackedWidget::currentChanged, [=](){
        if(ui->windows->currentWidget() != ui->settingsPage) lastPage = ui->windows->currentWidget();
    });
    connect(this, &Window::sendMesToStausbar, this, &Window::receiveMesToStatusbar);
    //    connect(ui->progressInstall, &QProgressBar::valueChanged, [&](int val) {
    //        taskBarProgress->setValue(val);
    //    });
    //connect(ui->progressDelete, &QProgressBar::valueChanged, taskBarProgress, &QWinTaskbarProgress::setValue);
    connect(log::getConsole(), &console::hided, [=](){
        ui->checkEnableConSettings->setChecked(false);
    });

    //ui->progressDelete->setHidden(true);
    ui->progressInstall->setAlignment(Qt::AlignCenter);
    ui->progressInstall->setValue(0);
    //ui->progressDelete->setRange(0, 7);
    //ui->progressDelete->setValue(0);
    ui->editSizeDataInstall->setValidator(new QIntValidator(100, 999999));
    ui->editDirForInstall->setValidator(new QRegExpValidator(QRegExp("[^а-яА-Я^ ]{0,999}")));
    global->insSet->execBars(ui->progressInstall, 0, ui->statusbar);
    ui->buttonInstallInstall->setShortcut(Qt::Key_Return);

    retranslateUi(QString::fromStdString(_langHelper::to_string(global->set->getLang())));

#if WIN
    setTaskProgress();
#endif

    if(first) global->set->autowrite_set();
    returnMainWindow();

    qDebug().noquote() << "------------- STARTED -------------";
}

void Window::setLabelSetInfo() {
    QString info = QString(global->set->tbios ? "Uefi " : "Bios ") + QString(global->set->arch ? "x64" : "x86");
    info = QString("<b>") + info + "<\b>";
    ui->labelSetInfo->setText(info);
}

void Window::retranslateUi(QString lang) {
    translator.load(QString("%1/translations/yourdroid_%2").arg(qApp->applicationDirPath(), lang));
    qDebug().noquote() << "Loaded the ts file";
    qApp->installTranslator(&translator);
    qDebug().noquote() << "Installed translator";
    ui->retranslateUi(this);
    qDebug().noquote() << "Retranslated ui";
    ui->labelVersion->setText(QString("<b>") + tr("Version:") + QString(" ") + global->VERSION + "<\b>");
    qDebug().noquote() << "Set the version label";
}

Window::~Window()
{
    delete ui;
}

void Window::returnMainWindow() {
    qDebug().noquote() << tr("Clearing systems list...");
    ui->systemsTree->blockSignals(true);
    ui->systemsTree->clear();
    ui->systemsTree->blockSignals(false);

    qDebug().noquote() << tr("Filling systems list...");
    for(auto sys : global->insSet->systemsVector())
    {
        QStringList itemInfo;
        itemInfo << sys.name << QString::fromStdString(_bootloaderHelper::to_string(sys.bootloader))
                 << QString::fromStdString(_typePlaceHelper::to_string(sys.typePlace))
                 << sys.place;
        qDebug().noquote() << "Adding a system: " << itemInfo;
        QTreeWidgetItem *item = new QTreeWidgetItem(itemInfo);
        ui->systemsTree->addTopLevelItem(item);
    }

    setWindowTitle(tr("YourDroid | Main menu"));
    ui->windows->setCurrentWidget(ui->mainWindowPage);
}

void Window::Settings_clicked()
{
    if(!first) global->set->read_set(false);
    ui->typeBios->setCurrentIndex((int)global->set->tbios);
    ui->arch->setCurrentIndex((int)global->set->arch);
    ui->downloadListCheck->setChecked(global->set->downloadList);
#if LINUX
    ui->winVer->setEnabled(false);
#endif
    ui->comboLanguageSettings->setCurrentIndex((int)global->set->getLang());
    //ui->applaySettings->setEnabled(false);
    ui->windows->setCurrentWidget(ui->settingsPage);
    setWindowTitle(tr("YourDroid | Settings"));
}

void Window::on_applaySettings_clicked()
{
    qDebug().noquote() << "Applying settings";

    QString lang;
    int langIndex = ui->comboLanguageSettings->currentIndex();
    switch(langIndex)
    {
    case 0: lang = "en"; break;
    case 1: lang = "ru"; break;
    default: lang = "en";
    }
    qDebug().noquote() << "Language index: " << langIndex << " Language: " << lang
                       << "Current language:" << translator.language();

    if(translator.language() != lang) {
        langChanged = false;
        qDebug().noquote() << "Retranslating";
        retranslateUi(lang);
    }
    global->set->write_set(true, ui->arch->currentIndex(),
                           ui->typeBios->currentIndex(),
                           (_lang)ui->comboLanguageSettings->currentIndex(),
                           ui->downloadListCheck->isChecked());
    setLabelSetInfo();
    restoredDefaults = false;
}

void Window::on_restoreSettings_clicked()
{
    qDebug().noquote() << "Restoring defaults";
    restoredDefaults = true;
    global->set->read_set(true);
    ui->typeBios->setCurrentIndex((int)global->set->tbios);
    ui->arch->setCurrentIndex((int)global->set->arch);
    ui->comboLanguageSettings->setCurrentIndex((int)global->set->getLang());
    ui->downloadListCheck->setChecked(global->set->downloadList);
}

void Window::on_buttonRefreshInstall_clicked()
{
#if WIN
    ui->comboDriveSelect->clear();
    QStringList mountedDrives = global->insSet->getDrives("where (drivetype!=5 and drivetype!=6)");
    if(!mountedDrives.isEmpty())
    {
        if(mountedDrives[0] == "error")
        {
            qWarning().noquote() << "Cannot get the mounted drives";
        }
        else
        {
            qDebug().noquote() << "Excluding c:/";
            mountedDrives.removeOne("C:\\");
            qDebug().noquote() << mountedDrives;
            if(global->set->tbios)
            {
                qDebug().noquote() << "Excluding the efi partition";
                mountedDrives.removeOne(global->set->efiMountPoint.toUpper() + '\\');
                qDebug().noquote() << mountedDrives;
            }
            ui->comboDriveSelect->addItems(mountedDrives);
        }
    }
    if(ui->comboDriveSelect->count() > 0)
        ui->radioInstallOnPart->setEnabled(true);
    else ui->radioInstallOnPart->setEnabled(false);


    ui->comboFlashDrivesInstall->clear();
    mountedDrives = global->insSet->getDrives("where drivetype=2");
    if(!mountedDrives.isEmpty())
    {
        if(mountedDrives[0] == "error")
        {
            qWarning().noquote() << "Cannot get the mounted removable drives";
        }
        else
        {
            for(int i = 0; i < mountedDrives.count(); i++)
            {
                if(QFile::exists(mountedDrives[i] + "yourdroid_usb_boot")
                        || QFile::exists(mountedDrives[i] + "yourdroid_usb_boot.cfg"))
                {
                    qDebug().noquote() << QString("%1 is a boot partition, skipping it")
                                          .arg(mountedDrives[i]);
                    mountedDrives.removeAt(i);
                    i--;
                }
            }
            ui->comboFlashDrivesInstall->addItems(mountedDrives);
        }
    }
    if(ui->comboFlashDrivesInstall->count() > 0)
        ui->radioInstallFlashDriveIns->setEnabled(true);
    else ui->radioInstallFlashDriveIns->setEnabled(false);
#elif LINUX
    ui->radioInstallOnPart->setEnabled(false);
    ui->radioInstallFlashDriveIns->setEnabled(false);
#endif
}

void Window::on_installButtonMain_clicked()
{
    setUpdating(false);

    setInstallPage();
}

void Window::setInstallPage()
{
    ui->radioInstallOnDir->setChecked(true);
    on_radioInstallOnDir_clicked();

    ui->radioDataToImg->setChecked(true);
    on_radioDataToImg_clicked();

    ui->radioDownload->setEnabled(false);

    ui->radioChooseFromDisk->setChecked(true);
    on_radioChooseFromDisk_clicked();

    //ui->progressInstall->setRange(0, 100);
    ui->comboBoot->clear();
//    if(!global->set->tbios) {
//        //ui->comboBoot->addItem("Grub legasy");
//        if(global->set->winv && global->set->os) ui->comboBoot->addItem("Grub4dos");
//    }
//    //if(!global->set->winv && global->set->os) ui->comboBoot->addItem("Windows NTLDR");
//    if(global->set->tbios) {
//        //ui->comboBoot->addItem("rEFInd");
//        ui->comboBoot->addItem("Grub2");
//    }
    bool replaceWinBtldr = false;
#if WIN
    if(!global->set->tbios) ui->comboBoot->addItem("Windows bootloader");
#endif
    ui->comboBoot->addItem("Grub2");
    if(global->set->tbios)
    {
#if WIN
        replaceWinBtldr = true;
#endif
        ui->comboBoot->addItem("Grub2 for tablets");

        if(updating == false)
        {
            qDebug().noquote() << QString("^%1|yn|").arg(QObject::tr("Is this a tablet?"));
            auto choice = log::getLastPressedButton();
            if(choice == QMessageBox::Yes)
            {
                qDebug().noquote() << "Yes. Setting grub2 for tablet the default and checking the "
                                      "\"Replace Windows Bootloader\" box ";
                ui->comboBoot->setCurrentText("Grub2 for tablets");
                ui->checkReplaceWinBootloader->setChecked(true);
            }
            else
            {
                qDebug().noquote() << "No. Setting grub2 the default";
                ui->comboBoot->setCurrentText("Grub2");
                ui->checkReplaceWinBootloader->setChecked(false);
            }
        }
    }

    ui->checkReplaceWinBootloader->setEnabled(replaceWinBtldr);

    emit on_buttonRefreshInstall_clicked();

    ui->comboVersionDown->clear();
    ui->radioDownload->setEnabled(false);
    QStringList androidNameList;
    if(!global->set->downloadList) androidListDownloaded = true;
    if(!androidListDownloaded)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel(this);
        QProgressBar *progressBar = new QProgressBar(this);
        QDialog dialog(this);

        layout->addWidget(label);
        layout->addWidget(progressBar);
        label->setText(QObject::tr("Downloading the list of android's urls"));
        progressBar->setRange(0, 100);
        dialog.setLayout(layout);
        dialog.setWindowTitle(QObject::tr("Downloading the list of android's urls"));

        Downloader *downloader = new Downloader(this);
        QObject *context = new QObject(this);

        auto downloadEnded = [&](){
            dialog.done(0);
        };

        QObject::connect(downloader, &Downloader::success, context, [&](){
            downloadEnded();
            qDebug().noquote() << "Android list is downloaded successfully";
            androidListDownloaded = true;
        });

        QObject::connect(downloader, &Downloader::error, context, [&](QString mess){
            downloadEnded();
            qCritical().noquote() << QObject::tr("^Error downloading the android list: %1").arg(mess);
            androidListDownloaded = false;
        });

        QObject::connect(downloader, &Downloader::updateDownloadProgress, context,
                         [&](qint64 received, qint64 total){
            int progress = received / total * 100;
            progressBar->setValue(progress);
        });

        downloader->get(qApp->applicationDirPath() + "/android_list.ini",
                       QUrl("https://raw.githubusercontent.com/YourDroid/Info/master/android_list.ini"));
        dialog.exec();
        delete context;
        delete downloader;
    }
    if(QFile::exists(qApp->applicationDirPath() + "/android_list.ini"))
    {
        QSettings s(qApp->applicationDirPath() + "/android_list.ini", QSettings::IniFormat);
        QStringList groups = s.childGroups();
        qDebug().noquote() << "Android list groups: " << groups;

        QStringList androidx86List;
        for(auto x : groups)
        {
            if(x.contains("Android"))
            {
                qDebug().noquote() << "Found an android x86 system: " << x;
                androidx86List.push_back(x);
                groups.removeOne(x);
            }
        }
        qDebug().noquote() << "Android x86 systems: " << androidx86List;

        groups = groups + androidx86List;
        qDebug().noquote() << "Sorted android list: " << groups;

        for(auto x : groups)
        {
            s.beginGroup(x);
            QUrl url = s.value("url").toString();
            qDebug().noquote() << QString("The url of %1 is %2").arg(x, url.toString());
            androidsToDownload.push_front(QPair<QString, QUrl>(x, url));
            androidNameList.push_front(x);
            s.endGroup();
        }
    }

    if(!androidNameList.isEmpty())
    {
        qDebug().noquote() << "The android list is not empty";
        ui->comboVersionDown->addItems(androidNameList);
        ui->radioDownload->setEnabled(true);
    }


    if(!updating) setWindowTitle(tr("YourDroid | Install"));
    else setWindowTitle(tr("YourDroid | Update %1")
                        .arg(global->insSet->systemsVector().begin()
                             [ui->systemsTree->selectionModel()->selectedIndexes()[0].row()].name));
    ui->windows->setCurrentWidget(ui->installPage);
}

void Window::on_buttonChooseImage_clicked()
{
    QString image = QFileDialog::getOpenFileName(0, tr("Choose image for install"), "", "*.iso");
    if(image.length() > 0) {
        ui->editImageFromDisk->setText(image);
        qDebug().noquote() << qApp->translate("log", "Choose image for install: ") + image;
    }
}

void Window::on_back_settings_clicked()
{
    emit on_applaySettings_clicked();
    if(first) {
        first = !first;
        //on_applaySettings_clicked();
        returnMainWindow();
    }
    //else if(lastPage == ui->settingsPage) Settings_clicked();
    else if(lastPage == ui->mainWindowPage) returnMainWindow();
    //else if(lastPage == ui->aboutPage) on_buttonAboutMain_clicked();
    else if(lastPage == ui->installPage) on_installButtonMain_clicked();
    //else if(lastPage == ui->deletePage) on_deleteButtonMain_clicked();
}

void Window::on_buttonChooseDirForInstall_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(0, tr("Choose directory for install"), "");
    if(dir.length() > 0) {
        ui->editDirForInstall->setText(dir);
        qDebug().noquote() << qApp->translate("log", "Choose dir for install: ") + dir;
    }
}

void Window::on_buttonInstallInstall_clicked()
{
    QObject *context = new QObject(this);

    QString installPath;
    if(ui->radioInstallOnPart->isChecked()) installPath = ui->comboDriveSelect->currentText();
    else if(ui->radioInstallFlashDriveIns->isChecked())
        installPath = ui->comboFlashDrivesInstall->currentText();
    else installPath = ui->editDirForInstall->text();
    installPath.replace('\\', '/');
    if(installPath.back() == '/') installPath.chop(1);
    qDebug().noquote() << QString("The install path is: %1").arg(installPath);

    QString message = QObject::tr("Are you sure?");
    if(ui->radioInstallFlashDriveIns->isChecked() &&
            !global->insSet->isUsbAlreadyFormatted(installPath))
    {
        message += ' ';
        message += QObject::tr("The flash drive is going to be formatted!");
        qWarning().noquote() << QString("^%1|+-|").arg(message);
    }
    else qDebug().noquote() << QString("^%1|+-|").arg(message);
    auto choice = log::getLastPressedButton();
    if(choice == QMessageBox::Ok)
    {
        qDebug().noquote() << "Yes";
    }
    else
    {
        qDebug().noquote() << "No. Canceling";
        return;
    }

    installing = true;

    ui->progressInstall->setStyleSheet(QProgressBar().styleSheet());
    ui->progressInstall->setValue(0);
    auto setBlocked = [=](bool _blocked)
    {
        if(_blocked) qDebug().noquote() << "Blocking the ui";
        else qDebug().noquote() << "Unblocking the ui";
        _blocked = !_blocked;
        ui->buttonsWidgetInstall->setEnabled(_blocked);
        ui->mainWidgetInstall->setEnabled(_blocked);
        ui->settingsMini->setEnabled(_blocked);
    };
    setBlocked(true);
    ui->statusbar->showMessage(tr("Checking"));
    qDebug().noquote() << tr("Checking data for install...");
    QString image, dir, name;
    auto end = [=, this](QString mess = QObject::tr("Ready")){
        ui->statusbar->showMessage(mess);
        setBlocked(false);
        qDebug().noquote() << "Killing the context object";
        delete context;
        this->installing = false;
    };
    connect(this, &Window::ending, context, [=](QString mess) {
        end(mess);
    });

//    auto retExpr = cmd::exec(QString("%1/data/iso-editor.exe")
//                             .arg(qApp->applicationDirPath()));
//    if(retExpr.second.contains("Too few arguments")) //It output this message if it works fine
//        qDebug().noquote() << QObject::tr("iso-editor works fine");
//    else
//    {
//        qCritical().noquote() << tr("^iso-editor doesn't work:\n%1").arg(retExpr.second);
//        end();
//        return;
//    }

    if(ui->radioInstallOnPart->isChecked() && !QFile::exists("C:/windows/INF/ext2fsd.inf"))
    {
        qDebug().noquote() << "C:/windows/INF/ext2fsd.inf doesn't exist";
        qCritical().noquote() << QObject::tr("^You should install Ext2fsd first to be able to install "
                                             "to a separate partition:"
                                             "https://sourceforge.net/projects/ext2fsd/files/latest/download");
        end();
        return;
    }
    else qDebug().noquote() << "C:/windows/INF/ext2fsd.inf exists";
    if(!ui->radioDownload->isChecked()) {
        if((image = ui->editImageFromDisk->text()).length() == 0) {
            qCritical().noquote() << tr("^The image is not chosen");
            end();
            return;
        }
        if(!QFile::exists(image)) {
            qCritical().noquote() << tr("^Chosen image does not exist");
            end();
            return;
        }

        if(!QFile(ui->editImageFromDisk->text()).open(QIODevice::ReadWrite)) {
            qCritical().noquote() << QObject::tr("^Can't access the image");
            end();
            return;
        }
        else qDebug().noquote() << QObject::tr("Successfully opened the image");
    }
    if(!updating)
    {
        if((dir = ui->editDirForInstall->text()).length() == 0 && ui->radioDataToFolder->isChecked()
                && !ui->radioInstallOnPart->isChecked() && !ui->radioInstallFlashDriveIns->isChecked()) {
            qCritical().noquote() << tr("^The folder is not chosen");
            end();
            return;
        }
        if(dir.contains(' '))
        {
            qCritical().noquote() << QObject::tr("^The installation path must not contain any spaces");
            end();
            return;
        }
        //    if((dir = ui->editDirForInstall->text()).length() == OS * 2 + 1 ) {
        //        qCritical().noquote() << tr("^Choosen folder is root");
        //        end();
        //        return;
        //    }
        if(!ui->editDirForInstall->hasAcceptableInput()) {
            qCritical().noquote() << tr("^Invalid path");
            end();
            return;
        }
        //    if(!(new QDir())->exists(dir)) {
        //        qCritical().noquote() << tr("^Selected folder does not exist");
        //        end();
        //        return;
        //    }
        if((name = ui->editName->text()).length() == 0) {
            qCritical().noquote() << tr("^Did not fill in the name");
            end();
            return;
        }
        if((name = ui->editSizeDataInstall->text()).length() == 0 &&
                !ui->radioDataToFolder->isChecked()) {
            qCritical().noquote() << tr("^Did not fill in the size of data.img");
            end();
            return;
        }
        for(int i = 0; i < global->insSet->systemsVector().length(); i++) {
            auto sys = (global->insSet->systemsVector())[i];
            if(ui->editName->text() == sys.name) {
                if((ui->radioInstallFlashDriveIns->isChecked() && sys.typePlace == _typePlace::flash_drive) ||
                        (!ui->radioInstallFlashDriveIns->isChecked() && sys.typePlace != _typePlace::flash_drive))
                {
                    qCritical().noquote() << QObject::tr("^The system with written name already exists");
                    end();
                    return;
                }
            }
        }
        //    if(!QDir(ui->editDirForInstall->text()).isEmpty()) { //if dir is not empty
        //        qWarning().noquote() << tr("^Choosen folder is not empty. Some files will overwrite. Press cancel to abort|+-|");
        //        if(log::getLastPressedButton() == QMessageBox::Cancel) {
        //            end();
        //            return;
        //        }
        //    }
    }

#if LINUX
    QPair<bool, QString> result = global->insSet->mountImage(ui->editImageFromDisk->text());
    if(!result.first) {
        qCritical().noquote() << QObject::tr("^Could not mount image: %1").arg(result.second);
        global->insSet->unmountImage();
        end();
        return;
    }
#endif

    if(!ui->radioDownload->isChecked())
    {
        int ret = 0;
        if(!(ret = global->insSet->isInvalidImage(
#if WIN
                 ui->editImageFromDisk->text()
#endif
                 ))) {
            if(ret != 2) qCritical().noquote() << QObject::tr("^The downloaded image doesn't contain "
                                                              "some of the required files");
            end();
            return;
        }
    }

    qDebug().noquote() << QObject::tr("Data of install is valid");
    ui->statusbar->showMessage(QObject::tr("Data of install is valid"));

#if WIN
    taskBarProgress->resume();
#endif

#if WIN
    if(global->set->getBios()) {
        ui->statusbar->showMessage(QObject::tr("Mounting efi partition"));
        if(global->set->isEfiPartMounted())
        {
            qDebug().noquote() << QObject::tr("The efi partition has already been mounted");
        }
        else if(!global->set->mountEfiPart().first && !ui->radioInstallFlashDriveIns->isChecked())
        {
            qCritical().noquote()
                    << QObject::tr("^Could not mount efi partition. Aborting. \nTry rebooting your computer");
            end();
            return;
        }
    }
#endif

    //ui->progressInstall->setRange(0, (ui->radioChooseFromDisk->isChecked() && !ui->radioDownload->isChecked()) ? 125 : 150);
    QString boot = ui->comboBoot->currentText();
    if(boot == "Windows bootloader") boot = "win_bootloader";
    else if(boot == "Grub2 for tablets") boot = "grub2_tablet";
    else boot = boot.toLower();
    if(ui->radioInstallFlashDriveIns->isChecked()) boot = "grub2_flash";
    qDebug().noquote() << "The bootloader is " << boot;
    _bootloader bootloader = _bootloaderHelper::from_string(boot.toStdString());

    _typePlace typePlace;
    if(ui->radioInstallOnDir->isChecked()) typePlace = _typePlace::dir;
    else if (ui->radioInstallOnPart->isChecked()) typePlace = _typePlace::partition;
    else if (ui->radioInstallFlashDriveIns->isChecked()) typePlace = _typePlace::flash_drive;
    //_typePlace typePlace = ui->radioInstallOnDir->isChecked() ? _typePlace::dir : _typePlace::partition;
#define CHECK_ABORT() if(abort) return;

    //    QWinTaskbarButton *taskBarButton = new QWinTaskbarButton(this);
    //    taskBarButton->setWindow(windowHandle());

    //    QWinTaskbarProgress *taskBarProgress = taskBarButton->progress();
    //    taskBarProgress->setVisible(true);

    //installPath is defined at the begining of this function

    if(!updating) global->insSet->addSystem(bootloader, typePlace, installPath, ui->editImageFromDisk->text(), ui->editName->text(), false);
    QFutureWatcher<void> *resMonitor = new QFutureWatcher<void>;
    connect(resMonitor, &QFutureWatcher<void>::finished, context, [&](){
        qApp->alert(this, 2000);
        if(aborted) {
            emit ending(QObject::tr("Aborted"));
            ui->progressInstall->setStyleSheet("QProgressBar::chunk {background-color: red;}");
#if WIN
            taskBarProgress->stop();
#endif
//            disconnect(global->insSet, &install::abort, this, nullptr);
//            androidDelete();
            return;
        }
        emit ending();
        global->insSet->sysSetSuccess();
//        global->insSet->write();
        ui->progressInstall->setValue(ui->progressInstall->maximum());
#if WIN
        taskBarProgress->hide();
        taskBarProgress->pause();
#endif
        on_buttonRefreshInstall_clicked();
        qDebug().noquote() << QObject::tr("^Success");
    });

    progressSteps = 3;
    currentSteps = 1;
    if(ui->radioInstallOnPart->isChecked() || ui->radioInstallFlashDriveIns->isChecked())
        progressSteps++;
    if(ui->radioDownload->isChecked()) progressSteps++;
    if(updating) progressSteps -= 2;
    int step = 100 / progressSteps;
    qDebug().noquote() << "The number of progress steps is " << progressSteps;
    qDebug().noquote() << "Progress step is " << step;

    ui->progressInstall->setRange(0, 105);
    ui->progressInstall->setValue(5);
#if WIN
    taskBarProgress->setRange(0, 105);
    taskBarProgress->setValue(5);
#endif

    connect(this, &Window::progressAddStep, context, [step, this](){
        qDebug().noquote() << "Current steps: " << currentSteps;
        if(currentSteps > progressSteps)
        {
            qWarning().noquote() << "Too many progress steps";
            return;
        }
        ui->progressInstall->setValue(5 + step * currentSteps);
#if WIN
        taskBarProgress->setValue(5 + step * currentSteps);
#endif
        currentSteps++;
    });
    connect(global->insSet, &install::downloadProgress, context, [step, this](qint64 received, qint64 total) {
        float progress = (received * 100) / total;
        qDebug().noquote() << "The download progress is " << progress;
        int totalProgress = 5 + step * (currentSteps - 1) + progress * step / 100;
        qDebug().noquote() << "Total progress is " << totalProgress;
        ui->progressInstall->setValue(totalProgress);
#if WIN
        taskBarProgress->setValue(totalProgress);
#endif
    }, Qt::QueuedConnection);

    auto logMain = [=](QtMsgType type, QString mess, bool installMsg){
        QDebug(type).noquote() << mess;
        qDebug().noquote() << "C'est tout bien!";
        if(installMsg) global->insSet->logWindowEnded = true;
    };
    connect(global->insSet, &install::logWindow, context, [=](QtMsgType type, QString mess) {
        emit logFromMainThread(type, mess, true);
    });
    connect(this, &Window::logFromMainThread, context, logMain);
    connect(this, &Window::setAborted, context, [=](bool a) {
        aborted = a;
    });


    if(aborted) {
        aborted = false;
#if WIN
        setTaskProgress();
#endif
    }
#if WIN
    taskBarProgress->setVisible(true);
    taskBarProgress->show();
#endif
    //taskBarProgress->setValue(step);
    auto res = QtConcurrent::run([=](){ // auto - QFuture
        bool abort = false;
        connect(global->insSet, &install::abort, context, [&](QString mes){
            abort = true;
            emit setAborted(true);
            global->insSet->delBackSystem();
            emit logFromMainThread(QtCriticalMsg,
                                   tr("^Fatal error while installing: %1")
                                   .arg(mes.isEmpty() ? "Message about error is empty" : mes), false);
#if LINUX
            global->insSet->unmountImage();
#endif
        });
        qDebug().noquote() << tr("Start install");

        if(ui->radioDownload->isChecked())
        {
            qDebug().noquote() << tr("Downloading the image...");
            emit sendMesToStausbar(tr("Downloading the image..."));
            QString androidName = ui->comboVersionDown->currentText();
            qDebug().noquote() << "Chosen image is " << androidName;
            QUrl url;
            for(auto x : androidsToDownload)
            {
                if(x.first == androidName)
                {
                    url = x.second;
                    qDebug().noquote() << QString("%1 matches the chosen image. The url is %2")
                                          .arg(x.first, x.second.toString());
                    break;
                }
                else qDebug().noquote() << QString("%1 doesn't match the chosen image")
                                           .arg(x.first);
            }
            if(url.isEmpty())
            {
                emit global->insSet->abort(tr("Can't find the url of the chosen image"));
            }
            else global->insSet->downloadImage(url);

            int ret = 0;
            if(!(ret = global->insSet->isInvalidImage(
#if WIN
                     qApp->applicationDirPath() + "/android.iso"
#endif
                     ))) {
                if(ret != 2) global->insSet->abort(tr("The downloaded image doesn't contain "
                                                      "some of the required files"));
            }

            CHECK_ABORT();
            emit progressAddStep();
        }

        if(ui->radioInstallFlashDriveIns->isChecked() && !updating)
        {
            qDebug().noquote() << tr("Formating flash drive and installing the bootloader (may take a while)...");
            emit sendMesToStausbar(tr("Formating flash drive and installing the bootloader (may take a while)..."));
            global->insSet->formatFlashDrive();
            CHECK_ABORT();
            emit progressAddStep();
        }
        else if(ui->radioInstallOnPart->isChecked() && !updating)
        {
            qDebug().noquote() << tr("Formating the selected drive...");
            emit sendMesToStausbar(tr("Formating the selected drive..."));
            global->insSet->formatPartExt4();
            CHECK_ABORT();
            emit progressAddStep();
        }

        int numSys = -1;
        if(updating)
            numSys = ui->systemsTree->selectionModel()->selectedIndexes()[0].row();
        qDebug().noquote() << tr("Unpacking iso...");
        emit sendMesToStausbar(tr("Unpacking iso..."));
        global->insSet->unpackSystem(static_cast<sysImgExtractType>
                                     (ui->comboSysMountAs->currentIndex()), numSys);
        CHECK_ABORT();
        emit progressAddStep();

        if(!updating)
        {
            qDebug().noquote() << tr("Creating data.img...");
            emit sendMesToStausbar(tr("Creating data.img..."));
            global->insSet->createDataImg(ui->editSizeDataInstall->text().toInt(),
                                          ui->radioDataToFolder->isChecked());
            CHECK_ABORT();
            emit progressAddStep();

            qDebug().noquote() << tr("Installing and configuring bootloader...");
            emit sendMesToStausbar(tr("Installing and configuring bootloader..."));
            global->insSet->registerBootloader(ui->checkReplaceWinBootloader->isChecked());
            CHECK_ABORT();
            emit progressAddStep();
        }

#if LINUX
        emit sendMesToStausbar(tr("Unmounting image..."));
        emit progressAddStep();
        global->insSet->unmountImage();
#endif
        global->insSet->systemEnd();
        global->insSet->oldSysEdit() = true;
        if(typePlace == _typePlace::flash_drive)
            global->insSet->saveFlashData();
        else
            global->insSet->write();
        CHECK_ABORT();
    });
    resMonitor->setFuture(res);
    return;
#undef CHECK_ABORT
}

void Window::on_buttonAboutMain_clicked()
{
    setWindowTitle(tr("YourDroid | About"));
    ui->windows->setCurrentWidget(ui->aboutPage);
}

void Window::on_comboBoot_currentIndexChanged(const QString &arg1)
{
    qDebug().noquote() << qApp->translate("log", "Choose ") + arg1;
    //    if(arg1 == "Grub2") {
    //        ui->labelAboutBootloader->setText("Рекомендуется для компьютеров.");
    //        ui->labelAboutBootloader_2->setText("Текстовый.");
    //    }
    //    else if(arg1 == "Gummiboot") {
    //        ui->labelAboutBootloader->setText("Рекомендуется для планшетов.");
    //        ui->labelAboutBootloader_2->setText("Управление качалкой громкости.");
    //    }
    //    else if(arg1 == "rEFInd") ui->labelAboutBootloader->setText("Поддержка сенс. экрана, работает не везде");
    //    else if(arg1 == "Windows BOOTMGR") ui->labelAboutBootloader->setText("Рекомендуется для планшетов, c win 8+. В win 8+ сенсорный");
    //    else if(arg1 == "Windows NTDLR") ui->labelAboutBootloader->setText("Не рекомендуется. Текстовый");
    //    else if(arg1 == "Grub legasy") ui->labelAboutBootloader->setText("Рекомендуется для старых компьютеров. Текстовый");
}

void Window::on_buttonDeleteDelete_clicked()
{
    if(ui->systemsTree->selectedItems().isEmpty()) return;
    qDebug().noquote() << QString("^%1|+-|").arg(QObject::tr("Are you sure?"));
    auto choice = log::getLastPressedButton();
    if(choice == QMessageBox::Ok)
    {
        qDebug().noquote() << "Yes";
    }
    else
    {
        qDebug().noquote() << "No. Canceling";
        return;
    }

#if WIN
    taskBarProgress->resume();
#endif

    androidDelete();

    global->insSet->write();

    qDebug().noquote() << QObject::tr("^Finished");
}

void Window::androidDelete()
{
    ui->buttonDeleteDelete->setEnabled(false);
    ui->settingsMini->setEnabled(false);
    ui->systemsTree->setEnabled(false);
    int num = ui->systemsTree->selectionModel()->selectedIndexes()[0].row();
    qDebug().noquote() << qApp->translate("log", "Deleting ") + (global->insSet->systemsVector().begin() + num)->name;
    global->insSet->delSystemFiles(num);
    global->insSet->deleteBootloaderEntry(num);
    global->insSet->oldSysEdit() = true;
    global->insSet->deleteSettingsEntry(num);
    //on_deleteButtonMain_clicked();
    returnMainWindow();
    ui->statusbar->showMessage(tr("Ready"));
    ui->buttonDeleteDelete->setEnabled(true);
    ui->settingsMini->setEnabled(true);
    ui->systemsTree->setEnabled(true);
}

void Window::receiveMesToStatusbar(QString mes) {
    ui->statusbar->showMessage(mes);
}

void Window::closeEvent(QCloseEvent *event) {
    if(installing)
    {
        QMessageBox::StandardButton resBtn = QMessageBox::warning(this, QObject::tr("Warning!"),
                                                                  tr("Are you sure you want to close YourDroid "
                                                                     "during installation?\n"
                                                                     "Please note that this will not stop it, "
                                                                     "you are just going to close the window and "
                                                                     "the installation is going to continue "
                                                                     "in the background!"),
                                                                  QMessageBox::Cancel |
                                                                  QMessageBox::Yes,
                                                                  QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
            return;
        }
    }

    log::setEnabledCon(false);
    exiting = true;
    event->accept();
}

void Window::changeEvent(QEvent *event) {
    //    if(event->type() == QEvent::WindowStateChange) emit deactived();
    //    else if(event->type() == QEvent::WindowActivate) emit actived();
    event->accept();
}

void Window::on_comboLanguageSettings_currentIndexChanged(int index)
{
    langChanged = true;
}

#if WIN
void Window::setTaskProgress() {
    if(taskBarButton != nullptr) delete taskBarButton;
    taskBarButton = new QWinTaskbarButton(this);
    taskBarButton->setWindow(windowHandle());
    taskBarProgress = taskBarButton->progress();
    taskBarProgress->setVisible(true);
}
#endif

void Window::on_radioButton_clicked()
{
    //ui->comboFlashDrivesInstall->setEnabled(true);
    ui->editDirForInstall->setEnabled(false);
    ui->buttonChooseDirForInstall->setEnabled(false);
}

void Window::on_radioInstallOnDir_clicked()
{
    ui->comboBoot->setEnabled(true);
    //ui->comboFlashDrivesInstall->setEnabled(false);
    ui->editDirForInstall->setEnabled(true);
    ui->buttonChooseDirForInstall->setEnabled(true);
    ui->checkReplaceWinBootloader->setEnabled(true);
    ui->radioDataToFolder->setEnabled(false);
    if(ui->radioDataToFolder->isChecked())
        ui->radioDataToImg->setChecked(true);

    ui->comboDriveSelect->setEnabled(false);
    ui->labelDriveSelect->setEnabled(false);

    ui->comboFlashDrivesInstall->setEnabled(false);

    ui->editDirForInstall->setEnabled(true);
    ui->buttonChooseDirForInstall->setEnabled(true);
}

void Window::on_radioInstallFlashDriveIns_clicked()
{
    ui->comboBoot->setEnabled(false);
    ui->checkReplaceWinBootloader->setEnabled(false);
    ui->radioDataToFolder->setEnabled(false);
    if(ui->radioDataToFolder->isChecked())
        ui->radioDataToImg->setChecked(true);

    ui->comboDriveSelect->setEnabled(false);
    ui->labelDriveSelect->setEnabled(false);

    ui->comboFlashDrivesInstall->setEnabled(true);

    ui->editDirForInstall->setEnabled(false);
    ui->buttonChooseDirForInstall->setEnabled(false);
}

void Window::on_radioInstallOnPart_clicked()
{
    ui->comboBoot->setEnabled(true);
    ui->checkReplaceWinBootloader->setEnabled(true);
    ui->radioDataToFolder->setEnabled(true);

    ui->comboDriveSelect->setEnabled(true);
    ui->labelDriveSelect->setEnabled(true);

    ui->comboFlashDrivesInstall->setEnabled(false);

    ui->editDirForInstall->setEnabled(false);
    ui->buttonChooseDirForInstall->setEnabled(false);
}

void Window::on_updateButtonMain_clicked()
{
    if(ui->systemsTree->selectedItems().isEmpty()) return;

    setUpdating(true);

    setInstallPage();
}

void Window::setUpdating(bool upd)
{
    updating = upd;

    ui->groupWhere->setVisible(!upd);
    ui->groupOtherInfo->setVisible(!upd);
    ui->radioDataToImg->setVisible(!upd);
    ui->radioDataToFolder->setVisible(!upd);
    ui->labelSizeDataInstall->setVisible(!upd);
    ui->editSizeDataInstall->setVisible(!upd);
    ui->line->setVisible(!upd);
}

void Window::on_checkEnableConSettings_stateChanged(int arg1)
{
    log::setEnabledCon(arg1, this);
}

void Window::on_radioDownload_clicked()
{
    ui->comboVersionDown->setEnabled(true);
    ui->labelVersionDown->setEnabled(true);

    ui->editImageFromDisk->setEnabled(false);
    ui->buttonChooseImage->setEnabled(false);
}

void Window::on_radioChooseFromDisk_clicked()
{
    ui->comboVersionDown->setEnabled(false);
    ui->labelVersionDown->setEnabled(false);

    ui->editImageFromDisk->setEnabled(true);
    ui->buttonChooseImage->setEnabled(true);
}

void Window::on_radioDataToImg_clicked()
{
    ui->editSizeDataInstall->setEnabled(true);
    ui->labelSizeDataInstall->setEnabled(true);
}

void Window::on_radioDataToFolder_clicked()
{
    ui->editSizeDataInstall->setEnabled(false);
    ui->labelSizeDataInstall->setEnabled(false);
}
