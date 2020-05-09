#include "consolereader.h"

#include <QtCore/QTextStream>

ConsoleReader::ConsoleReader(QObject *parent) : m_notifier(fileno(stdin), QSocketNotifier::Read)
{
    QObject::connect(&m_notifier, &QSocketNotifier::activated, this, &ConsoleReader::text);
}

void ConsoleReader::text()
{
    QTextStream qin(stdin);
    QString line = qin.readLine();
    emit textReceived(line);
}
