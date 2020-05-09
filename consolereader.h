#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H

#include <QtCore/QObject>
#include <QtCore/QSocketNotifier>

class ConsoleReader : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleReader(QObject *parent = Q_NULLPTR);

Q_SIGNALS:
    void textReceived(const QString &message);

private Q_SLOTS:
    void text();

private:
    QSocketNotifier m_notifier;
};


#endif // CONSOLEREADER_H
