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

Window::Window(bool f, QWidget *parent) :
    QMainWindow(parent),
    fierst(!f),
    ui(new Ui::Window)
{
    if(global->set->tbios == false && OS == 0) {
        qCritical().noquote() << tr("^This PC does not supported");
    }

    //setWindowIcon(QIcon(":/yourdroid.png"));

    qDebug().noquote() << tr("Setting window");

    ui->setupUi(this);
    setLabelSetInfo();

#if WIN
    if (QtWin::isCompositionEnabled())
        QtWin::extendFrameIntoClientArea(this, 0, 0, 0, 0);
    else
        QtWin::resetExtendedFrame(this);
#endif

    connect(ui->returnInstallButton,SIGNAL(clicked()),SLOT(returnMainWindow()));
    connect(ui->settingsMini,&QPushButton::clicked,[=](){
        if(ui->windows->currentWidget() == ui->settingsPage) on_back_settings_clicked();
        else Settings_clicked();
    });
    connect(ui->buttonAboutReturn,SIGNAL(clicked()),SLOT(returnMainWindow()));
    connect(ui->buttonReturnMainDelete,SIGNAL(clicked()),SLOT(returnMainWindow()));
    connect(ui->windows, &QStackedWidget::currentChanged, [=](){
        if(ui->windows->currentWidget() != ui->settingsPage) lastPage = ui->windows->currentWidget();
    });
    connect(this, &Window::sendMesToStausbar, this, &Window::receiveMesToStatusbar);
    //    connect(ui->progressInstall, &QProgressBar::valueChanged, [&](int val) {
    //        taskBarProgress->setValue(val);
    //    });
    //connect(ui->progressDelete, &QProgressBar::valueChanged, taskBarProgress, &QWinTaskbarProgress::setValue);

    ui->progressInstall->setStyleSheet("QProgressBar::chunk {background-color: green;}");
    ui->progressInstall->setAlignment(Qt::AlignCenter);
    ui->progressInstall->setValue(0);
    ui->progressDelete->setRange(0, 7);
    ui->progressDelete->setValue(0);
    ui->editSizeDataInstall->setValidator(new QIntValidator(100, 999999));
    ui->editDirForInstall->setValidator(new QRegExpValidator(QRegExp("[^а-яА-Я^ ]{0,999}")));
    global->insSet->execBars(ui->progressInstall, ui->progressDelete, ui->statusbar);
    ui->buttonInstallInstall->setShortcut(Qt::Key_Return);

    retranslateUi(QString::fromStdString(_langHelper::to_string(global->set->getLang())));

#if WIN
    setTaskProgress();
#endif

    if(fierst) Settings_clicked();
    else returnMainWindow();


}

void Window::setLabelSetInfo() {
    QString info = QString(global->set->tbios ? "Uefi " : "Bios ") + QString(global->set->arch ? "x64" : "x86");
#if WIN
    info = QString((global->set->winv ? "win. vista+ | " : "win. xp | ")) + info;
#endif
    info = QString("<b>") + info + "<\b>";
    ui->labelSetInfo->setText(info);
}

void Window::retranslateUi(QString lang) {
    QTranslator translator;
    translator.load(QString("yourdroid_") + lang);
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
    ui->labelVersion->setText(QString("<b>") + tr("Version:") + QString(" ") + global->VERSION + "<\b>");
}

Window::~Window()
{
    delete ui;
}

void Window::returnMainWindow() {
    setWindowTitle(tr("YourDroid | Main menu"));
    ui->windows->setCurrentWidget(ui->mainWindowPage);
}

void Window::Settings_clicked()
{
    if(!fierst) global->set->read_set(false);
    ui->typeBios->setCurrentIndex((int)global->set->tbios);
    ui->arch->setCurrentIndex((int)global->set->arch);
    ui->winVer->setCurrentIndex((int)global->set->winv);
#if LINUX
    ui->winVer->setEnabled(false);
#endif
    ui->checkEnableConSettings->setChecked(global->set->getConEnable());
    ui->comboLanguageSettings->setCurrentIndex((int)global->set->getLang());
    //ui->applaySettings->setEnabled(false);
    ui->windows->setCurrentWidget(ui->settingsPage);
    setWindowTitle(tr("YourDroid | Settings"));
}

void Window::on_applaySettings_clicked()
{
    if(langChanged) {
        langChanged = false;
        qInfo().noquote() << tr("^For applying language application should restart");
        //        retranslateUi(QString::fromStdString(_langHelper::to_string(
        //                                                 (_lang)ui->comboLanguageSettings->currentIndex())));
    }
    global->set->write_set(true, ui->arch->currentIndex(),
                           ui->typeBios->currentIndex(),
                           ui->winVer->currentIndex(),
                           ui->checkEnableConSettings->checkState() == Qt::Checked,
                           (_lang)ui->comboLanguageSettings->currentIndex());
    log::setEnabledCon(ui->checkEnableConSettings->checkState() == Qt::Checked);
    setLabelSetInfo();
}

void Window::on_restoreSettings_clicked()
{
    global->set->read_set(true);
    ui->typeBios->setCurrentIndex((int)global->set->tbios);
    ui->arch->setCurrentIndex((int)global->set->arch);
    ui->winVer->setCurrentIndex((int)global->set->winv);
    ui->checkEnableConSettings->setChecked(global->set->getConEnable());
    ui->comboLanguageSettings->setCurrentIndex((int)global->set->getLang());
}

void Window::on_installButtonMain_clicked()
{
    ui->comboFlashDrivesInstall->setEnabled(false);
    ui->radioInstallOnPart->setEnabled(false);
    ui->radioInstallOnDir->setChecked(true);

    ui->radioDataToImg->setChecked(true);

    ui->radioDownload->setEnabled(false);
    ui->radioChooseFromDisk->setChecked(true);

    //ui->progressInstall->setRange(0, 100);

    setWindowTitle(tr("YourDroid | Install"));
    ui->windows->setCurrentWidget(ui->installPage);
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

    ui->comboBoot->addItem("Grub2");
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
    if(fierst) {
        fierst = !fierst;
        on_applaySettings_clicked();
        returnMainWindow();
    }
    else if(lastPage == ui->settingsPage) Settings_clicked();
    else if(lastPage == ui->mainWindowPage) returnMainWindow();
    else if(lastPage == ui->aboutPage) on_buttonAboutMain_clicked();
    else if(lastPage == ui->installPage) on_installButtonMain_clicked();
    else if(lastPage == ui->daletePage) on_deleteButtonMain_clicked();
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
    ui->progressInstall->setStyleSheet("QProgressBar::chunk {background-color: green;}");
    ui->progressInstall->setValue(0);
    auto setBlocked = [=](bool _blocked)
    {
        _blocked = !_blocked;
        ui->returnInstallButton->setEnabled(_blocked);
        ui->buttonInstallInstall->setEnabled(_blocked);
    };
    setBlocked(true);
    ui->statusbar->showMessage(tr("Checking"));
    qDebug().noquote() << tr("Checking data for install...");
    QString image, dir, name;
    auto end = [=](QString mess = QObject::tr("Ready")){
        ui->statusbar->showMessage(mess);
        setBlocked(false);
    };
    connect(this, &Window::ending, this, [=](QString mess) {
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

    if(!QFile(ui->editImageFromDisk->text()).open(QIODevice::ReadWrite)) {
        qCritical().noquote() << QObject::tr("^Can't access the image");
        end();
        return;
    }
    else qDebug().noquote() << QObject::tr("Successfully opened the image");

    if((image = ui->editImageFromDisk->text()).length() == 0) {
        qCritical().noquote() << tr("^The image is chosen");
        end();
        return;
    }
    if(!QFile::exists(image)) {
        qCritical().noquote() << tr("^Chosen image does not exist");
        end();
        return;
    }
    if((dir = ui->editDirForInstall->text()).length() == 0 ) {
        qCritical().noquote() << tr("^The folder is not chosen");
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
        if(ui->editName->text() == (global->insSet->systemsVector())[i].name) {
            qCritical().noquote() << QObject::tr("^The system with written name already exists");
            end();
            return;
        }
    }
//    if(!QDir(ui->editDirForInstall->text()).isEmpty()) { //if dir is not empty
//        qWarning().noquote() << tr("^Choosen folder is not empty. Some files will overwrite. Press cancel to abort|+-|");
//        if(log::getLastPressedButton() == QMessageBox::Cancel) {
//            end();
//            return;
//        }
//    }

#if LINUX
    QPair<bool, QString> result = global->insSet->mountImage(ui->editImageFromDisk->text());
    if(!result.first) {
        qCritical().noquote() << QObject::tr("^Could not mount image: %1").arg(result.second);
        global->insSet->unmountImage();
        end();
        return;
    }
#endif

    int ret = 0;
    if(!(ret = global->insSet->isInvalidImage(
         #if WIN
             ui->editImageFromDisk->text()
         #endif
             ))) {
        if(ret != 2) qCritical().noquote() << QObject::tr("^Image has not needed files");
        end();
        return;
    }

    qDebug().noquote() << QObject::tr("Data of install is valid");
    ui->statusbar->showMessage(QObject::tr("Data of install is valid"));

    ui->progressInstall->setValue(5);
#if WIN
    taskBarProgress->setValue(5);
#endif

#if WIN
    if(global->set->getBios()) {
        ui->statusbar->showMessage(QObject::tr("Mounting efi partition"));
        if(global->set->isEfiPartMounted())
        {
            qDebug().noquote() << QObject::tr("The efi partition has already been mounted");
        }
        else if(!global->set->mountEfiPart().first)
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
    if(boot == "Windows NTLDR") boot = "ntldr";
    else if(ui->radioInstallFlashDriveIns->isChecked()) boot = "grub2_flash";
    else boot = boot.toLower();
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

    global->insSet->addSystem(bootloader, typePlace, ui->editDirForInstall->text(), ui->editImageFromDisk->text(), ui->editName->text(), false);
    QFutureWatcher<void> *resMonitor = new QFutureWatcher<void>;
    connect(resMonitor, &QFutureWatcher<void>::finished, this, [&](){
        qApp->alert(this, 2000);
        if(aborted) {
            emit ending(QObject::tr("Aborted"));
            ui->progressInstall->setStyleSheet("QProgressBar::chunk {background-color: red;}");
#if WIN
            taskBarProgress->stop();
#endif
            return;
        }
        emit ending();
        ui->progressInstall->setValue(ui->progressInstall->maximum());
#if WIN
        taskBarProgress->hide();
        taskBarProgress->stop();
#endif
        qDebug().noquote() << QObject::tr("^Success");
    });

    int step = 33;
    ui->progressInstall->setRange(0, 100);
#if WIN
    taskBarProgress->setRange(0, 100);
#endif

    qDebug().noquote() << QObject::tr("Progress step is %1").arg(step);

    connect(this, &Window::progressAddStep, this, [&, step](){
        ui->progressInstall->setValue(ui->progressInstall->value() + step);
#if WIN
        taskBarProgress->setValue(taskBarProgress->value() + step);
#endif
    });
    auto logMain = [=](QtMsgType type, QString mess){
        QDebug(type).noquote() << mess;
    };
    connect(global->insSet, &install::logWindow, this, [=](QtMsgType type, QString mess) {
        emit logFromMainThread(type, mess);
    });
    connect(this, &Window::logFromMainThread, this, logMain);
    connect(this, &Window::setAborted, this, [=](bool a) {
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
        connect(global->insSet, &install::abort, [&](QString mes){
            emit setAborted(true);
            global->insSet->delBackSystem();
            abort = true;
            emit logFromMainThread(QtCriticalMsg,
                                   tr("^Fatal error while installing: %1")
                                   .arg(mes.isEmpty() ? "Message about error is empty" : mes));
#if LINUX
            global->insSet->unmountImage();
#endif
        });
        qDebug().noquote() << tr("Start install");

        if(ui->radioInstallFlashDriveIns->isChecked())
        {
            qDebug().noquote() << tr("Formating flash drive...");
            emit sendMesToStausbar(tr("Formating flash drive..."));
            global->insSet->formatFlashDrive();
            CHECK_ABORT();
        }

        qDebug().noquote() << tr("Unpacking iso...");
        emit sendMesToStausbar(tr("Unpacking iso..."));
        global->insSet->unpackSystem(static_cast<sysImgExtractType>
                                     (ui->comboSysMountAs->currentIndex()));
        CHECK_ABORT();
        emit progressAddStep();

        qDebug().noquote() << tr("Creating data.img...");
        emit sendMesToStausbar(tr("Creating data.img..."));
        global->insSet->createDataImg(ui->editSizeDataInstall->text().toInt(),
                                      ui->radioDataToFolder->isChecked());
        CHECK_ABORT();
        emit progressAddStep();

        qDebug().noquote() << tr("Installing bootloader...");
        emit sendMesToStausbar(tr("Installing bootloader..."));
        global->insSet->registerBootloader();
        CHECK_ABORT();
        emit progressAddStep();

#if LINUX
        emit sendMesToStausbar(tr("Unmounting image..."));
        emit progressAddStep();
        global->insSet->unmountImage();
#endif
        global->insSet->systemEnd();
        global->insSet->oldSysEdit() = true;
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

void Window::on_deleteButtonMain_clicked()
{
    ui->progressDelete->setValue(0);
    ui->labelPlaceDeleteText->clear();
    ui->labelbootloaderDeleteText->clear();

    qDebug().noquote() << tr("Clearing systems list...");
    ui->comboSystemDelete->blockSignals(true);
    ui->comboSystemDelete->clear();
    ui->comboSystemDelete->blockSignals(false);

    qDebug().noquote() << tr("Filling systems list...");
    for(auto sys : global->insSet->systemsVector()) ui->comboSystemDelete->addItem(sys.name);

    setWindowTitle(tr("YourDroid | Delete"));
    ui->windows->setCurrentWidget(ui->daletePage);
}

void Window::on_comboSystemDelete_currentIndexChanged(int index)
{
    ui->labelPlaceDeleteText->setText(global->insSet->systemsVector()[index].place);
    ui->labelbootloaderDeleteText->setText(QString::fromStdString(_bootloaderHelper::to_string(global->insSet->systemsVector()[index].bootloader)));
}
void Window::on_buttonDeleteDelete_clicked()
{
    if(ui->comboSystemDelete->currentIndex() == -1) return;
    ui->buttonDeleteDelete->setEnabled(false);
    ui->buttonReturnMainDelete->setEnabled(false);
    ui->settingsMini->setEnabled(false);
    ui->comboSystemDelete->setEnabled(false);
    int num = ui->comboSystemDelete->currentIndex();
    qDebug().noquote() << qApp->translate("log", "Deleting ") + (global->insSet->systemsVector().begin() + num)->name;
    global->insSet->delSystemFiles(num);
    global->insSet->deleteBootloaderEntry(num);
    global->insSet->oldSysEdit() = true;
    global->insSet->deleteSettingsEntry(num);
    on_deleteButtonMain_clicked();
    ui->statusbar->showMessage(tr("Ready"));
    ui->buttonDeleteDelete->setEnabled(true);
    ui->buttonReturnMainDelete->setEnabled(true);
    ui->settingsMini->setEnabled(true);
    ui->comboSystemDelete->setEnabled(true);
}

void Window::receiveMesToStatusbar(QString mes) {
    ui->statusbar->showMessage(mes);
}

void Window::closeEvent(QCloseEvent *event) {
    exiting = true;
    emit closed();
    event->accept();
}

void Window::changeEvent(QEvent *event) {
    //    if(event->type() == QEvent::WindowStateChange) emit deactived();
    //    else if(event->type() == QEvent::WindowActivate) emit actived();
    event->accept();
}

void Window::consoleHided() {
    if(exiting) return;
    global->set->setConEnable(false);
    global->set->autowrite_set();
    ui->checkEnableConSettings->setChecked(false);
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
    ui->comboFlashDrivesInstall->setEnabled(true);
    ui->editDirForInstall->setEnabled(false);
    ui->buttonChooseDirForInstall->setEnabled(false);
}

void Window::on_radioInstallOnDir_clicked()
{
    ui->comboFlashDrivesInstall->setEnabled(false);
    ui->editDirForInstall->setEnabled(true);
    ui->buttonChooseDirForInstall->setEnabled(true);
}
