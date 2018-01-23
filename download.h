#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QUrl>
#include <qDebug().noquote()>
#include <functional>

class download : public QObject
{
    Q_OBJECT
public:
    explicit download(QUrl, QFile);

signals:
    void ready();
    void progress(int, int);
    void error(QString);

public slots:
    void getData();     // Метод инициализации запроса на получение данных
    void onResult();    // Слот обработки ответа о полученных данных

private:
    QNetworkAccessManager *manager; // менеджер сетевого доступа
    QNetworkReply *reply;
    QUrl url;
    QFile path;
};
