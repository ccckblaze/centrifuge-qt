#ifndef CENTRIFUGE_H
#define CENTRIFUGE_H

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtWebSockets/QtWebSockets>

class Centrifuge : public QObject
{
    Q_OBJECT
public:
    explicit Centrifuge(const QString &url, const QString &user, const QString &timestamp, const QString &token, QObject *parent = Q_NULLPTR);

    void subscribeChannel(const QString &channel);
    void   publishChannel(const QString &channel, const QString &message);
    void   historyChannel(const QString &channel);


Q_SIGNALS:
    void       error(const QString &error);

    void    dispatch(const QJsonObject &msg);

    void    connect_(const QJsonObject &body);
    void  disconnect(const QJsonObject &body);
    void   subscribe(const QJsonObject &body);
    void unsubscribe(const QJsonObject &body);
    void     publish(const QJsonObject &body);
    void    presence(const QJsonObject &body);
    void     history(const QJsonObject &body);
    void        join(const QJsonObject &body);
    void       leave(const QJsonObject &body);
    void        ping(const QJsonObject &body);
    void     refresh(const QJsonObject &body);
    void     message(const QJsonObject &body);


private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    void send(const QJsonObject &cmd);

    void dispatchMessage(const QJsonObject &message);

    void     connectResponse(const QJsonObject &message);
    void  disconnectResponse(const QJsonObject &message);
    void   subscribeResponse(const QJsonObject &message);
    void unsubscribeResponse(const QJsonObject &message);
    void     publishResponse(const QJsonObject &message);
    void    presenceResponse(const QJsonObject &message);
    void     historyResponse(const QJsonObject &message);
    void        joinResponse(const QJsonObject &message);
    void       leaveResponse(const QJsonObject &message);
    void        pingResponse(const QJsonObject &message);
    void     refreshResponse(const QJsonObject &message);
    void     messageResponse(const QJsonObject &message);

    bool checkError(const QJsonObject &message);

    void connectServer();

    QString nextUID();

private:
    int m_uid;
    QString m_user;
    QString m_timestamp;
    QString m_token;
    QString m_client;
    QUrl m_url;
    QWebSocket m_webSocket;
};

#endif // CENTRIFUGE_H
