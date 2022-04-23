#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QtNetwork>

enum PROTOCOL
{
    TCP = 0,
    UDP = 1
};

class Network
{
    public: Network(int _protocol);
    public: virtual ~Network();
    public: virtual void Send() = 0;
    public: virtual void Receive() = 0;

    protected: quint16 serverPort;
    protected: std::string serverAddress;
    protected: int protocol;
};


class TcpNetwork: public Network
{
    public: TcpNetwork();
    public: void Send() override;
    public: void Receive() override;

    private: QTcpSocket socket;
};

class UdpNetwork: public Network
{
    public: UdpNetwork();
    public: void Send() override;
    public: void Receive() override;

    private: QUdpSocket socket;
};



#endif // NETWORK_H
