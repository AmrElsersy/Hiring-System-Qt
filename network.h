#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QHostAddress>
#include <QAbstractSocket>

#include <QJsonDocument>
#include <QJsonObject>

#include <QObject>
#include <QTimer>

class NetworkClient : public QObject
{
    Q_OBJECT
    public: NetworkClient(std::string _address, quint16 _port);
    public: virtual ~NetworkClient();
    public: virtual void Send(QJsonObject);
    public: virtual QJsonObject Receive();
    public: QAbstractSocket *GetSocket();

public slots:
    void ConnectToServer();

    private: quint16 serverPort;
    private: std::string serverAddress;
    private: QTcpSocket *socket;
    private: QTimer *reconnectTimer;
    private: bool isReconnected;

signals:
    void connected();
};


#endif // NETWORK_H
