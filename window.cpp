#include "window.h"
#include "ui_window.h"
#include "enum.h"
#include <string>
#include "install.h"
#include <QValidator>

Window::Window(install *ins, bool f, options *d, QWidget *parent) :
    QMainWindow(parent),
    fierst(!f),
    dat(d),
    insDat(ins),
    ui(new Ui::Window)
{
    if(dat->tbios == false || OS) {
        log::message(2, __FILE__, __LINE__, "This PC does not supported", "Ваш компьютер пока не поддерживается");
    }

    //setWindowIcon(QIcon(":/yourdroid.png"));

    log::message(0, __FILE__, __LINE__, "Start window");

    ui->setupUi(this);
    setLabelSetInfo();

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

    ui->labelVersion->setText(QString("<b>Версия: ") + VERSION + QString("<\b>"));
    ui->editSizeDataInstall->setValidator(new QIntValidator(100, 999999));
    ui->editDirForInstall->setValidator(new QRegExpValidator(QRegExp("[^а-яА-Я]{0,999}")));

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
    else if(lastPage == ui->settingsPage) Settings_clicked();
    else if(lastPage == ui->mainWindowPage) returnMainWindow();
    else if(lastPage == ui->aboutPage) on_buttonAboutMain_clicked();
    else if(lastPage == ui->installPage) on_installButtonMain_clicked();
    else if(lastPage == ui->daletePage) on_deleteButtonMain_clicked();
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
    ui->progressInstall->setValue(0);
    ui->returnInstallButton->setEnabled(false);
    ui->buttonInstallInstall->setEnabled(false);
    ui->statusbar->showMessage("Проверка");
    log::message(0, __FILE__, __LINE__, "Checking data for install...");
    QString image, dir, name;
    bool exit = false;
    if((image = ui->editImageFromDisk->text()).length() == 0) {
        log::message(2, __FILE__, __LINE__, "Not chosen image", "Выберите образ для установки!");
        exit = true;
    }
    else if(!QFile::exists(image)) {
        log::message(2, __FILE__, __LINE__, "Choosen image does not exist", "Выбранный образ не существует!");
        exit = true;
    }
    else if((dir = ui->editDirForInstall->text()).length() == 0 ) {
        log::message(2, __FILE__, __LINE__, "Not chosen folder", "Выберите папку для установки!");
        exit = true;
    }
    else if((dir = ui->editDirForInstall->text()).length() == OS * 2 + 1 ) {
        log::message(2, __FILE__, __LINE__, "Choosen folder is root", "Нельзя устанавливать в корень!");
        exit = true;
    }
    else if(!ui->editDirForInstall->hasAcceptableInput()) {
        log::message(2, __FILE__, __LINE__, "Invalid path", "Неправильный путь! В пути для установки нельзя использовать кирилицу!");
        exit = true;
    }
    else if(!(new QDir())->exists(dir)) {
        log::message(2, __FILE__, __LINE__, "Selected folder does not exist", "Выбранная папка не существует!");
        exit = true;
    }
    else if((name = ui->editName->text()).length() == 0) {
        log::message(2, __FILE__, __LINE__, "Not written the name", "Напишите имя!");
        exit = true;
    }
    else if((name = ui->editSizeDataInstall->text()).length() == 0) {
        log::message(2, __FILE__, __LINE__, "Not written the size of data.img", "Напишите размер data.img!");
        exit = true;
    }
    else for(int i = 0; i < insDat->systemsVector().length(); i++) {
        if(ui->editName->text() == (insDat->systemsVector())[i].name) {
            log::message(2, __FILE__, __LINE__, "The system with writеen name already exists", "Уже существует система с таким именем!");
            exit = true;
        }
    }
    if(exit) {
        ui->statusbar->clearMessage();
        ui->returnInstallButton->setEnabled(true);
        ui->buttonInstallInstall->setEnabled(true);
        return;
    }

    log::message(0, __FILE__, __LINE__, "Data for install valid");
    ui->statusbar->clearMessage();

    ui->progressInstall->setRange(0, (ui->radioChooseFromDisk->isChecked() && !ui->radioDownload->isChecked()) ? 125 : 150);
    QString boot = ui->comboBoot->currentText();
    if(boot == "Grub legasy") boot = "Grub_legasy";
    else if(boot == "Windows NTLDR") boot = "NTLDR";
    else if(boot == "Windows BOOTMGR") boot = "BOOTMGR";
    _bootloader bootloader = _bootloaderHelper::from_string(boot.toStdString());
    _typePlace typePlace = ui->radioInstallOnDir->isChecked() ? _typePlace::dir : _typePlace::partition;
    insDat->addSystem(bootloader, typePlace, ui->editDirForInstall->text(), ui->editImageFromDisk->text(), ui->editName->text());
    insDat->write();
    insDat->unpackSystem(ui->progressInstall, ui->statusbar);
    insDat->createDataImg(ui->editSizeDataInstall->text().toInt());
    insDat->installBootloader();
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
    if(arg1 == "Grub2") {
        ui->labelAboutBootloader->setText("Рекомендуется для компьютеров.");
        ui->labelAboutBootloader_2->setText("Текстовый.");
    }
    else if(arg1 == "Gummiboot") {
        ui->labelAboutBootloader->setText("Рекомендуется для планшетов.");
        ui->labelAboutBootloader_2->setText("Управление качалкой громкости.");
    }
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
