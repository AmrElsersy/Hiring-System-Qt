#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QHostAddress>
#include <QAbstractSocket>

#include <QJsonDocument>
#include <QJsonObject>

#include <QObject>

class NetworkServer : public QObject
{
    Q_OBJECT
public:
    NetworkServer();

private:
    QTcpServer *server;

};

#endif // SERVER_H
