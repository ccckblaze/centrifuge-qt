#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QNetworkAccessManager>

#include "centrifuge.h"
#include "consolereader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Centrifuge client("ws://127.0.0.1:8000/connection/websocket", "755893969", "1478689064", "bff2e4002d5a4efddfb8c3e460d4e21b0e4bbfcbe8b7a461e852315a3c774965");

    QObject::connect(&client, &Centrifuge::connect_, [&client](const QJsonObject &body) {
        client.subscribeChannel("public:testchat");
    });

    QObject::connect(&client, &Centrifuge::subscribe, [&client](const QJsonObject &body) {
        client.historyChannel(body["channel"].toString());
    });

    QObject::connect(&client, &Centrifuge::history, [](const QJsonObject &body) {
        const QString &channel = body["channel"].toString();
        const QJsonArray &messages = body["data"].toArray();
        foreach(const QJsonValue &val, messages) {
            const QJsonObject &msg = val.toObject()["data"].toObject();
            qDebug() << channel << "|" << msg["nick"].toString() << ":" << msg["message"].toString();
        }
    });

    QObject::connect(&client, &Centrifuge::message, [](const QJsonObject &body) {
        const QString &channel = body["channel"].toString();
        const QJsonObject &msg = body["data"].toObject();
        qDebug() << channel << "|" << msg["nick"].toString() << ":" << msg["message"].toString();
    });



    QNetworkRequest request(QString("http://t2.4399youpai.com/tv-chat.html?action=save"));
    {
        QString cookieHeader = "Pauth=755893969%7Cqzchenwl%7Cc0bd6de34d1b7b98abac54f1bf271d96%7C1478598593%7C0%7Ccea7e7f1fa29e401b63786ec795ec98a%7C; Pnick=%E2%95%AE%28%E2%95%AF_%E2%95%B0%29%E2%95%AD%E6%98%B5%E7%A7%B0; Hm_lvt_baa16a691f8e47d7f96dc058f64449f2=1478598593; Hm_lpvt_baa16a691f8e47d7f96dc058f64449f2=1478744827";
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader(QString("Cookie").toUtf8(), cookieHeader.toUtf8());
    }
    QNetworkAccessManager manager;
    QObject::connect(&manager, &QNetworkAccessManager::finished, [](QNetworkReply *reply) { reply->deleteLater(); });

    ConsoleReader reader;

    QObject::connect(&reader, &ConsoleReader::textReceived, [&manager, &request](const QString &line) {
        qDebug() << "INPUT" << ":" << line;

        QUrlQuery postData;
        postData.addQueryItem("content", line);

        qDebug() << "POST" << ":" << postData.toString(QUrl::FullyEncoded);

        QNetworkReply *reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

        QObject::connect(reply, &QNetworkReply::readyRead, [reply]() {
            QString content = reply->readAll();
            qDebug() << "REPLY" << ":" << content;
        });
    });

    return a.exec();
}
