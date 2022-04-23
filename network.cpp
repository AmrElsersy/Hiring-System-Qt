#include "network.h"

Network::Network(int _protocol)
{
    this->protocol = _protocol;
}

Network::~Network()
{

}

////////////////////// TCP /////////////////////////////////
TcpNetwork::TcpNetwork(): Network (PROTOCOL::TCP)
{
    QHostAddress address;
    address.setAddress(QString::fromStdString(this->serverAddress));
    this->socket.bind(address, this->serverPort);
}

void TcpNetwork::Send()
{
//    this->socket.write();
}

void TcpNetwork::Receive()
{
    QByteArray bytes = this->socket.readAll();
}

////////////////////// UDP /////////////////////////////////
UdpNetwork::UdpNetwork(): Network (PROTOCOL::UDP)
{
    QHostAddress address;
    address.setAddress(QString::fromStdString(this->serverAddress));
    this->socket.bind(address, this->serverPort);
}

void UdpNetwork::Send()
{

}

void UdpNetwork::Receive()
{

}
