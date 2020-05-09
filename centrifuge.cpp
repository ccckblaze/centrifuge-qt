#include "centrifuge.h"

#include <QtCore/QJsonDocument>

Centrifuge::Centrifuge(const QString &url, const QString &user, const QString &timestamp, const QString &token, QObject *parent) :
    QObject(parent), m_user(user), m_timestamp(timestamp), m_token(token), m_url(url)
{
    qDebug() << "SERVER: " << url;

    connect(&m_webSocket, &QWebSocket::connected, this, &Centrifuge::onConnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &Centrifuge::onTextMessageReceived);

    m_webSocket.open(m_url);
}

void Centrifuge::connectServer()
{
    QJsonObject params;
    params["user"] = m_user;
    params["timestamp"] = m_timestamp;
    params["token"] = m_token;
    params["info"] = "";

    QJsonObject cmd;
    cmd["uid"] = nextUID();
    cmd["method"] = "connect";
    cmd["params"] = params;

    send(cmd);
}

void Centrifuge::subscribeChannel(const QString &channel)
{
    QJsonObject params;
    params["channel"] = channel;

    QJsonObject cmd;
    cmd["uid"] = nextUID();
    cmd["method"] = "subscribe";
    cmd["params"] = params;

    send(cmd);
}

void Centrifuge::publishChannel(const QString &channel, const QString &message)
{
    QJsonObject params;
    params["channel"] = channel;
    params["data"] = message;

    QJsonObject cmd;
    cmd["uid"] = nextUID();
    cmd["method"] = "publish";
    cmd["params"] = params;

    send(cmd);
}

void Centrifuge::historyChannel(const QString &channel)
{
    QJsonObject params;
    params["channel"] = channel;

    QJsonObject cmd;
    cmd["uid"] = nextUID();
    cmd["method"] = "history";
    cmd["params"] = params;

    send(cmd);
}

void Centrifuge::onConnected()
{
    qDebug() << "CONNECTED";
    connectServer();
}

void Centrifuge::onTextMessageReceived(QString message)
{
    qDebug() << "RECEIVED: " << message;
    const QJsonDocument json = QJsonDocument::fromJson(message.toUtf8());
    if (json.isArray()) {
        const QJsonArray &msgs = json.array();
        foreach(const QJsonValue &msg, msgs) {
            dispatchMessage(msg.toObject());
        }
    } else if (json.isObject()) {
        dispatchMessage(json.object());
    }
}

void Centrifuge::send(const QJsonObject &cmd)
{
    QString str(QJsonDocument(cmd).toJson(QJsonDocument::Compact));
    qDebug() << "SENDING: " << str;

    m_webSocket.sendTextMessage(str);
}

void Centrifuge::dispatchMessage(const QJsonObject &message)
{
    emit dispatch(message);

    const QJsonValue &methodJson = message["method"];
    if (!methodJson.isString()) {
        qDebug() << "dispatch: got message with empty method";
        return;
    }

    const QString &method = methodJson.toString();
    if (method == "connect") {
        connectResponse(message);
    } else if (method == "disconnect") {
        disconnectResponse(message);
    } else if (method == "subscribe") {
        subscribeResponse(message);
    } else if (method == "unsubscribe") {
        unsubscribeResponse(message);
    } else if (method == "publish") {
        publishResponse(message);
    } else if (method == "presence") {
        presenceResponse(message);
    } else if (method == "history") {
        historyResponse(message);
    } else if (method == "join") {
        joinResponse(message);
    } else if (method == "leave") {
        leaveResponse(message);
    } else if (method == "ping") {
        pingResponse(message);
    } else if (method == "refresh") {
        refreshResponse(message);
    } else if (method == "message") {
        messageResponse(message);
    } else {
        qDebug() << "dispatch: got message with unknown method " << method;
    }
}

void Centrifuge::connectResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    m_client = message["body"].toObject()["client"].toString();
    emit connect_(message["body"].toObject());
}

void Centrifuge::disconnectResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit disconnect(message["body"].toObject());
}

void Centrifuge::subscribeResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit subscribe(message["body"].toObject());
}

void Centrifuge::unsubscribeResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit unsubscribe(message["body"].toObject());
}

void Centrifuge::publishResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit publish(message["body"].toObject());
}

void Centrifuge::presenceResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit presence(message["body"].toObject());
}

void Centrifuge::historyResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit history(message["body"].toObject());
}

void Centrifuge::joinResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit join(message["body"].toObject());
}

void Centrifuge::leaveResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit leave(message["body"].toObject());
}

void Centrifuge::pingResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit ping(message["body"].toObject());
}

void Centrifuge::refreshResponse(const QJsonObject &message)
{
    if (checkError(message)) return;
    emit refresh(message["body"].toObject());
}

void Centrifuge::messageResponse(const QJsonObject &msg)
{
    if (checkError(msg)) return;
    emit message(msg["body"].toObject());
}

bool Centrifuge::checkError(const QJsonObject &message)
{
    if (message["error"].isString()) {
        emit error(message["error"].toString());
        return true;
    }
    return false;
}

QString Centrifuge::nextUID()
{
    return QString::number(++m_uid);
}
