#include "window.h"
#include "ui_window.h"
#include "enum.h"
#include <string>
#include "install.h"

Window::Window(install *ins, bool f, options *d, QWidget *parent) :
    QMainWindow(parent),
    fierst(!f),
    dat(d),
    insDat(ins),
    ui(new Ui::Window)
{
    lastPage = 0;

    setWindowIcon(QIcon(":/icon/yourdroid.png"));

    log::message(0, __FILE__, __LINE__, "Start window");

    ui->setupUi(this);
    setLabelSetInfo();

    connect(ui->returnInstallButton,SIGNAL(clicked()),SLOT(returnMainWindow()));
    connect(ui->settingsMini,SIGNAL(clicked()),SLOT(Settings_clicked()));
    connect(ui->buttonAboutReturn,SIGNAL(clicked()),SLOT(returnMainWindow()));
    connect(ui->buttonReturnMainDelete,SIGNAL(clicked()),SLOT(returnMainWindow()));

    ui->labelVersion->setText(QString("<b>Версия: ") + VERSION + QString("<\b>"));

    if(fierst) Settings_clicked();
    else returnMainWindow();


}

void Window::setLabelSetInfo() {
    QString info = QString(dat->tbios ? "Uefi " : "Bios ") + QString(dat->arch ? "x64" : "x86");
#if OS == 1
    info = QString((dat->winv ? "win. vista+ | " : "win. xp | ")) + info;
#endif
    info = QString("<b>") + info + "<\b>";
    ui->labelSetInfo->setText(info);
}

Window::~Window()
{
    delete ui;
}

void Window::returnMainWindow() {
    setWindowTitle("YourDroid | Главное меню");
    ui->windows->setCurrentWidget(ui->mainWindowPage);
}

void Window::Settings_clicked()
{
    if(ui->windows->currentIndex() == 1) return;
    lastPage = ui->windows->currentIndex();
    //dat->read_set(false);
    ui->typeBios->setCurrentIndex((int)dat->tbios);
    ui->arch->setCurrentIndex((int)dat->arch);
    ui->winVer->setCurrentIndex((int)dat->winv);
#if OS == 0
    ui->winVer->setEnabled(false);
#endif
    ui->windows->setCurrentWidget(ui->settingsPage);
    setWindowTitle("YourDroid | Настройки");
}

void Window::on_applaySettings_clicked()
{
    dat->write_set(true,ui->arch->currentIndex(),ui->typeBios->currentIndex(),ui->winVer->currentIndex());
    setLabelSetInfo();
}

void Window::on_restoreSettings_clicked()
{
    dat->read_set(true);
    ui->typeBios->setCurrentIndex((int)dat->tbios);
    ui->arch->setCurrentIndex((int)dat->arch);
    ui->winVer->setCurrentIndex((int)dat->winv);
}

void Window::on_installButtonMain_clicked()
{
    ui->radioInstallOnPart->setEnabled(false);
    ui->radioInstallOnDir->setChecked(true);

    ui->radioDownload->setEnabled(false);
    ui->radioDownloadDataimgInstall->setEnabled(false);
    ui->radioChooseFromDisk->setChecked(true);

    //ui->progressInstall->setRange(0, 100);
    ui->progressInstall->setValue(0);

    setWindowTitle("YourDroid | Установка");
    ui->windows->setCurrentWidget(ui->installPage);
    ui->comboBoot->clear();
    if(!dat->tbios) {
        ui->comboBoot->addItem("Grub legasy");
        //if(dat->winv && dat->os) ui->comboBoot->addItem("Windows BOOTMGR");
    }
    //if(!dat->winv && dat->os) ui->comboBoot->addItem("Windows NTLDR");
    if(dat->tbios) {
        //ui->comboBoot->addItem("rEFInd");
        if(dat->os) ui->comboBoot->addItem("Gummiboot");
        else ui->comboBoot->addItem("Grub2");
    }
}

void Window::on_buttonChooseImage_clicked()
{
    QString image = QFileDialog::getOpenFileName(0, "Выберите образ для установки", "", "*.iso");
    if(image.length() > 0) {
        ui->editImageFromDisk->setText(image);
        log::message(0, __FILE__, __LINE__, QString("Choose image for install: ") + image);
    }
}

void Window::on_back_settings_clicked()
{
    if(fierst) {
        fierst = !fierst;
        dat->autowrite_set();
        returnMainWindow();
    }
    else switch(lastPage) {
    case 0: returnMainWindow(); break;
    case 2: on_installButtonMain_clicked(); break;
    case 3: on_deleteButtonMain_clicked(); break;
    case 4: on_buttonAboutMain_clicked(); break;
    }
}

void Window::on_buttonChooseDirForInstall_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(0, "Выберите директории для установки", "");
    if(dir.length() > 0) {
        ui->editDirForInstall->setText(dir);
        log::message(0, __FILE__, __LINE__, QString("Choose dir for install: ") + dir);
    }
}

void Window::on_buttonInstallInstall_clicked()
{
//    auto error = [](QString mess, QString enmess) {

//    };

    ui->returnInstallButton->setEnabled(false);
    ui->buttonInstallInstall->setEnabled(false);
    ui->statusbar->showMessage("Проверка");
    log::message(0, __FILE__, __LINE__, "Checking data for install...");
    QString image, dir, name;
    bool exit = false;
    if((image = ui->editImageFromDisk->text()).length() == 0) {
        (new QErrorMessage(this))->showMessage("Выберите образ для установки!");
        exit = true;
    }
    else if(!QFile::exists(image)) {
        (new QErrorMessage(this))->showMessage("Выбранный образ не существует!");
        exit = true;
    }
    else if((dir = ui->editDirForInstall->text()).length() < 4) {
        (new QErrorMessage(this))->showMessage("Выберите папку для установки!");
        exit = true;
    }
    else if(!(new QDir())->exists(dir)) {
        (new QErrorMessage(this))->showMessage("Выбранная папка не существует!");
        exit = true;
    }
    else if((name = ui->editName->text()).length() == 0) {
        (new QErrorMessage(this))->showMessage("Выберите имя!");
        exit = true;
    }
    else for(int i = 0; i < insDat->systemsVector().length(); i++) {
        if(ui->editName->text() == (insDat->systemsVector())[i].name) {
            (new QErrorMessage(this))->showMessage("Уже сущуствует система с таким именем!");
            exit = true;
        }
    }
    if(exit) {
        log::message(0, __FILE__, __LINE__, "Data for install invalid");
        ui->statusbar->clearMessage();
        return;
    }

    log::message(0, __FILE__, __LINE__, "Data for install valid");
    ui->statusbar->clearMessage();

    QString boot = ui->comboBoot->currentText();
    if(boot == "Grub legasy") boot = "Grub_legasy";
    else if(boot == "Windows NTLDR") boot = "NTLDR";
    else if(boot == "Windows BOOTMGR") boot = "BOOTMGR";
    _bootloader bootloader = _bootloaderHelper::from_string(boot.toStdString());
    _typePlace typePlace = ui->radioInstallOnDir->isChecked() ? _typePlace::dir : _typePlace::partition;
    insDat->addSystem(bootloader, typePlace, ui->editDirForInstall->text(), ui->editImageFromDisk->text(), ui->editName->text());
    insDat->write();
    insDat->unpackSystem(ui->progressInstall);
    //insDat.installBootloader();
    ui->returnInstallButton->setEnabled(true);
    ui->buttonInstallInstall->setEnabled(true);
}

void Window::on_buttonAboutMain_clicked()
{
    setWindowTitle("YourDroid | О программе");
    ui->windows->setCurrentWidget(ui->aboutPage);
}

void Window::on_comboBoot_currentIndexChanged(const QString &arg1)
{
    log::message(0,__FILE__,__LINE__,QString("Choose ") + arg1);
    if(arg1 == "Grub2") ui->labelAboutBootloader->setText("Рекомендуется для компьютеров. Текстовый");
    else if(arg1 == "Gummiboot") ui->labelAboutBootloader->setText("Рекомендуется для планшетов. Управление качалкой громкости");
    else if(arg1 == "rEFInd") ui->labelAboutBootloader->setText("Поддержка сенс. экрана, работает не везде");
    else if(arg1 == "Windows BOOTMGR") ui->labelAboutBootloader->setText("Рекомендуется для планшетов, c win 8+. В win 8+ сенсорный");
    else if(arg1 == "Windows NTDLR") ui->labelAboutBootloader->setText("Не рекомендуется. Текстовый");
    else if(arg1 == "Grub legasy") ui->labelAboutBootloader->setText("Рекомендуется для старых компьютеров. Текстовый");
}

void Window::on_deleteButtonMain_clicked()
{
    ui->progressDelete->setRange(0, 100);
    ui->progressDelete->setValue(0);

    for(int i = 0; i < insDat->cntSys(); i++) ui->comboSystemDelete->addItem(insDat->systemsVector()[i].name);

    setWindowTitle("YourDroid | Удаление");
    ui->windows->setCurrentWidget(ui->daletePage);
}

void Window::on_comboSystemDelete_currentIndexChanged(int index)
{
    ui->labelPlaceDeleteText->setText(insDat->systemsVector()[index].place);
    ui->labelbootloaderDeleteText->setText(QString::fromStdString(_bootloaderHelper::to_string(insDat->systemsVector()[index].bootloader)));
}
