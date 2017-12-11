#ifndef WINDOW_H
#define WINDOW_H

#include "data.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QDialog>
#include <QStringList>
#include <QFileDialog>
#include <QErrorMessage>
#include <log.h>
#include "install.h"
#include <QCoreApplication>
#include <QComboBox>

extern const QString VERSION;

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(install*, bool, options*, QWidget *parent = 0);

    void setLabelSetInfo();

    ~Window();

signals:
    void closed();

    void sendMesToStausbar(QString);

public slots:
    void consoleHided();

    void returnMainWindow();

    void Settings_clicked();

    void on_qApplaySettings_clicked();

    void on_restoreSettings_clicked();

    void on_installButtonMain_clicked();

    void on_buttonAboutMain_clicked();

    void on_back_settings_clicked();

private slots:

//    void on_exitButton_clicked();

//    void on_returnInstallButton_clicked();

    void receiveMesToStatusbar(QString);

    void on_buttonChooseImage_clicked();

    void on_buttonChooseDirForInstall_clicked();

    void on_buttonInstallInstall_clicked();

    void on_comboBoot_currentIndexChanged(const QString &arg1);

    void on_deleteButtonMain_clicked();

    void on_comboSystemDelete_currentIndexChanged(int index);

    void on_buttonDeleteDelete_clicked();

    void on_comboLanguageSettings_currentIndexChanged(int index);

private:
    void closeEvent(QCloseEvent *);
    void changeEvent(QEvent *);
    bool fierst;
    bool exiting = false;
    bool langChanged = false;
    options *dat;
    install *insDat;
    Ui::Window *ui;
    QWidget *lastPage;
};

#endif // WINDOW_H
