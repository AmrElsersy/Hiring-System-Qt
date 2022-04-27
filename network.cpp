#include "network.h"

NetworkClient::NetworkClient(std::string _address, quint16 _port): QObject (nullptr)
{
    this->socket = new QTcpSocket();
    this->serverPort = _port;
    this->serverAddress = _address;

    this->reconnectTimer = new QTimer();
    this->reconnectTimer->setInterval(1000);
    connect(this->reconnectTimer, SIGNAL(timeout()), this, SLOT(ConnectToServer()));
    this->isReconnected = false;
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::Send(QJsonObject json)
{
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytes = document.toJson();
    this->socket->write(bytes);
    if (this->socket->waitForBytesWritten(100))
        qDebug() << "Sent" << bytes;
    else
        qDebug() << "Write failed";
}

QJsonObject NetworkClient::Receive()
{
    QByteArray bytes = this->socket->readAll();
    QJsonDocument document = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObject = document.object();
    qDebug() << "Received " << jsonObject.keys();
    return jsonObject;
}

QAbstractSocket *NetworkClient::GetSocket()
{
    return this->socket;
}

void NetworkClient::ConnectToServer()
{
    QHostAddress address;
    address.setAddress(QString::fromStdString(this->serverAddress));

    this->socket->connectToHost(address, this->serverPort);
    if (this->socket->waitForConnected())
    {
        qDebug() << "Connected to " << address << " on port " << this->serverPort;
        if (this->reconnectTimer->isActive())
            this->reconnectTimer->stop();
        if (this->isReconnected)
            emit connected();
    }
    else
    {
        qDebug() << "Failed to connect";
        if (!this->reconnectTimer->isActive())
            this->reconnectTimer->start();
        this->isReconnected = true;
    }

}

