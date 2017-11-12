#include "download.h"

download::download(QUrl u, QFile f) :
    url(u),
    path(f)
{
    // Инициализируем менеджер ...
    manager = new QNetworkAccessManager();
    // ... и подключаем сигнал о завершении получения данных к обработчику полученного ответа
    connect(reply, &QNetworkReply::finished, this, &download::onResult);
}

void download::start()
{
    reply = manager->get(QNetworkRequest(url)),     // Выполняем запрос
    connect(reply, &QNetworkReply::downloadProgress, [=](int fierst, int second){
        emit progress(fierst, second);
    });
}

void download::onResult()
{
    // Если в процесе получения данных произошла ошибка
    if(reply->error()){
        emit error(reply->errorString());
    } else {
        // В противном случае создаём объект для работы с файлом
        QFile *file = new QFile("C:/example/file.xml");
        // Создаём файл или открываем его на перезапись ...
        if(file->open(QFile::WriteOnly)){
            file->write(reply->readAll());  // ... и записываем всю информацию со страницы в файл
            file->close();                  // закрываем файл
        emit ready(); // Посылаем сигнал о завершении получения файла
        }
    }
}
