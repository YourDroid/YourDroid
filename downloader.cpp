#include "downloader.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>

Downloader::Downloader(QObject* parent) :
    BaseClass(parent)
{
    // Подключаемся к сигналу finished
    connect(&m_manager, &QNetworkAccessManager::finished, this, &Downloader::onReply);
}

bool Downloader::get(const QString& targetFile, const QUrl& url)
{
    qDebug().noquote() << "Downloading " << url.toString();
    if (targetFile.isEmpty() || url.isEmpty())
    {
        qCritical().noquote() << "Target file is empty or the url is";
        emit error(QObject::tr("Target file is empty or the url is"));
        emit ended();
        return false;
    }

    // Cоздаём объект класса файла для скачивания
    // здесь имеется целевая директория и имя файла, которое выделяется из URL
    m_file = new QFile(targetFile);
    // Пробуем открыть файл
    if (!m_file->open(QIODevice::WriteOnly))
    {
        qCritical().noquote() << "Error opening the file";
        emit error(QObject::tr("Error opening the file"));
        emit ended();
        delete m_file;
        m_file = nullptr;
        return false;
    }

    // Создаём запрос
    QNetworkRequest request(url);
    // Обязательно разрешаем переходить по редиректам
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    qDebug() << "11111";
    // Запускаем скачивание
    m_currentReply = m_manager.get(request);
    qDebug() << "666";

    // После чего сразу подключаемся к сигналам о готовности данных к чтению и обновлению прогресса скачивания
    connect(m_currentReply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
    connect(m_currentReply, &QNetworkReply::downloadProgress, this, &Downloader::updateDownloadProgress);
    return true;
}

void Downloader::onReadyRead()
{
    //qDebug().noquote() << "onReadyRead";
    // Если есть данные и файл открыт
    if (m_file)
    {
        // записываем их в файл
        m_file->write(m_currentReply->readAll());
    }
}

void Downloader::cancelDownload()
{
    // Отмена запроса
    if (m_currentReply)
    {
        m_currentReply->abort();
    }
}

void Downloader::onReply(QNetworkReply* reply)
{
    qDebug().noquote() << "onReply";
    // По завершению запроса
    if (reply->error() == QNetworkReply::NoError)
    {
        // сохраням файл
        m_file->flush();
        m_file->close();
        emit success();
    }
    else
    {
        // Или удаляем его в случае ошибки
        m_file->remove();
        emit error(reply->errorString());
    }

    emit ended();
    delete m_file;
    m_file = nullptr;
    reply->deleteLater();
}
