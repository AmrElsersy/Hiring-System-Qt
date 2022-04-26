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

#include <job.h>

// Thread for handling connection with the server
class CandidateConnThread: public QThread
{
    Q_OBJECT
public:
    explicit CandidateConnThread(QObject *parent = nullptr);
    void run();
    void SetSocketDescriptor(int);
    void SendToServer(QJsonObject);

private:
    QTcpSocket *connSocket;
    int socketDescriptor;

public slots:
    void onReadyRead();
    void onDisconnected();
    void onFeedback(std::string);

signals:
    void submitRequest(QJsonObject);
    std::vector<Job> jobsRequest();
};

class NetworkServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit NetworkServer(quint16 port, QObject *parent = nullptr);

protected:
    virtual void incomingConnection(int socketDescriptor);

public slots:
};

#endif // SERVER_H
