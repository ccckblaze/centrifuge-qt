# Centrifuge Qt Client

## Depandency

- [QtWebSockets](http://doc.qt.io/qt-5/qtwebsockets-index.html)

## How to use

```cpp
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>

#include "centrifuge.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Centrifuge client("ws://127.0.0.1:8000/connection/websocket", "755893969", "1478689064", "bff2e4002d5a4efddfb8c3e460d4e21b0e4bbfcbe8b7a461e852315a3c774965");

    QObject::connect(&client, &Centrifuge::connect_, [&](const QJsonObject &body) {
        client.subscribeChannel("public:testchat");
    });

    QObject::connect(&client, &Centrifuge::subscribe, [&](const QJsonObject &body) {
        client.historyChannel(body["channel"].toString());
    });

    QObject::connect(&client, &Centrifuge::history, [&](const QJsonObject &body) {
        const QJsonArray &messages = body["data"].toArray();
        foreach(const QJsonValue &val, messages) {
            const QJsonObject &msg = val.toObject()["data"].toObject();
            qDebug() << msg["nick"].toString() << ":" << msg["message"].toString();
        }
    });

    QObject::connect(&client, &Centrifuge::message, [&](const QJsonObject &body) {
        qDebug() << body["data"].toObject()["nick"].toString() << ":" << body["data"].toObject()["message"].toString();
    });

    return a.exec();
}
```

## Protocal

[Centrifuge](https://fzambia.gitbooks.io/centrifugal/content/server/client_protocol.html)
