#ifndef WINDOW_H
#define WINDOW_H

#include "options.h"
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

#if WIN
#include <QtWinExtras>
QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)
#endif

extern const QString VERSION;

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(bool, QWidget *parent = 0);

    ~Window();

signals:
    void ending(QString = QObject::tr("Ready"));

    void setAborted(int);

    void logFromMainThread(QtMsgType, QString, bool);

    void proccessFinished();

    void closed();

    void sendMesToStausbar(QString);

    void progressAddStep();

public slots:
    void returnMainWindow();

    void Settings_clicked();

    void on_applaySettings_clicked();

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

    void on_buttonDeleteDelete_clicked();

    void on_comboLanguageSettings_currentIndexChanged(int index);

    void on_radioButton_clicked();

    void on_radioInstallOnDir_clicked();

    void on_buttonRefreshInstall_clicked();

    void on_radioInstallFlashDriveIns_clicked();

    void on_radioInstallOnPart_clicked();

    void on_updateButtonMain_clicked();

    void on_checkEnableConSettings_stateChanged(int arg1);

private:
    void setInstallPage();

    void setUpdating(bool);

    void androidDelete();

    void setLabelSetInfo();

    void retranslateUi(QString);

#if WIN
    void setTaskProgress();

    QWinTaskbarButton *taskBarButton = nullptr;
    QWinTaskbarProgress *taskBarProgress = nullptr;
#endif
    bool aborted = false;
    void closeEvent(QCloseEvent *);
    void changeEvent(QEvent *);
    QTranslator translator;
    bool first;
    bool exiting = false;
    bool langChanged = false;
    //int thisIsTablet = 2; //0 - no; 1 - yes; 2 - not set yet
    Ui::Window *ui = nullptr;
    QWidget *lastPage = nullptr;
    QList<QPair<QString, QUrl>> androidsToDownload;
    bool androidListDownloaded = false;
    int progressSteps = 0;
    int currentSteps = 1;
    bool updating = false;
    bool installing = false;
    bool restoredDefaults = false;
};

#endif // WINDOW_H
