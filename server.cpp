#include "server.h"

NetworkServer::NetworkServer(quint16 port, QObject *parent) :QTcpServer(parent)
{
    // initlaize server and listen
    if(this->listen(QHostAddress("127.0.0.1"), port))
        qDebug() << "Server is listening on port " << port;
    else
        qDebug() << "Failed to initiate server on port " << port;

}

void NetworkServer::incomingConnection(int socketDescriptor)
{
    // create thread and set its socket descriptor (for conn communication)
    CandidateConnThread *thread = new CandidateConnThread();
    thread->SetSocketDescriptor(socketDescriptor);

    // connect thread with outside world

    // start thread
    thread->start();
}


CandidateConnThread::CandidateConnThread(QObject *parent): QThread (parent)
{
}

void CandidateConnThread::run()
{
    // assign the socket to the socket id descriptor
    this->connSocket = new QTcpSocket();
    this->connSocket->setSocketDescriptor(this->socketDescriptor);

    connect(this->connSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(this->connSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    // wait till the thread is terminated manully
    this->exec();
}

void CandidateConnThread::SetSocketDescriptor(int _socketDescriptor)
{
    this->socketDescriptor = _socketDescriptor;
}

void CandidateConnThread::SendToServer(QJsonObject json)
{
    QJsonDocument document;
    document.setObject(json);
    QByteArray bytes = document.toJson();
    this->connSocket->write(bytes);
    if (this->connSocket->waitForBytesWritten(100))
        qDebug() << "Thread " << this->socketDescriptor << " Send" << bytes;
    else
        qDebug() << "Write failed from thread " << this->socketDescriptor;
}

void CandidateConnThread::onReadyRead()
{
    // read from socket and handle response
    QByteArray bytes = this->connSocket->readAll();
    QJsonDocument document = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObjectRequest = document.object();


    if (jsonObjectRequest["type"] == "jobs")
    {
        // get jobs and send them back
        QJsonObject response;
        response["type"] = "jobs";

        this->SendToServer(response);
    }
    else if (jsonObjectRequest["type"] == "submit")
    {
        // just save that
    }
}

void CandidateConnThread::onFeedback(std::string feedback)
{
    // send feedback via conn socket
    QJsonObject response;
    response["type"] = "feedback";
    response["feedback"] = QJsonValue(QString::fromStdString(feedback));

    this->SendToServer(response);
}

void CandidateConnThread::onDisconnected()
{
    // free the conn socket and terminate the thread
    this->connSocket->deleteLater();
    this->exit(0);
}
