#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QNetworkAccessManager>

class QNetworkReply;
class QFile;

class Downloader : public QObject
{
    Q_OBJECT
    using BaseClass = QObject;

public:
    explicit Downloader(QObject* parent = nullptr);

    // Метод для запуска скачиввания
    bool get(const QString& targetFolder, const QUrl& url);

public slots:
    // Метод отмены загрузки
    void cancelDownload();

signals:
    // Сигнал передающий информацию о прогрессе загрузки
    void updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    // Слот для постепенного считывания загружаемых данных
    void onReadyRead();
    // Слот для обработки завершения запроса
    void onReply(QNetworkReply* reply);

private:
    QNetworkReply* m_currentReply {nullptr};    // Текущий обрабатываемый запрос
    QFile* m_file                 {nullptr};    // Текущий файл в который идёт запись
    QNetworkAccessManager m_manager;            // Сетевой менеджер для загрузки файлов
};

#endif // DOWNLOADER_H
