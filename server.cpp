#include "server.h"

NetworkServer::NetworkServer(quint16 port, QObject *parent) :QTcpServer(parent)
{
    // initlaize server and listen
    if(this->listen(QHostAddress("127.0.0.1"), port))
        qDebug() << "Server is listening on port " << port;
    else
        qDebug() << "Failed to initiate server on port " << port;

}

void NetworkServer::incomingConnection(qintptr socketDescriptor)
{
    // create thread and set its socket descriptor (for conn communication)
    ConnectionThread *thread = new ConnectionThread(socketDescriptor);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // connect thread with outside world

    // start thread
    thread->start();
}

//////////////////////////////////////////////////////////////////////////////

ConnectionThread::ConnectionThread(qintptr socketDescriptor, QObject *parent): QThread (parent)
{
    // assign the socket to the socket id descriptor
    this->connSocket = new QTcpSocket();
    this->connSocket->setSocketDescriptor(socketDescriptor);
}

void ConnectionThread::run()
{
    connect(this->connSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(this->connSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    qDebug() << "Created new thread for socket conn " << this->socketDescriptor;

    // wait till the thread is terminated manully
    this->exec();
}

void ConnectionThread::SendToClient(QJsonObject json)
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

void ConnectionThread::onReadyRead()
{
    // read from socket and handle response
    QByteArray bytes = this->connSocket->readAll();
    QJsonDocument document = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObjectRequest = document.object();

    qDebug() << "Server received " << jsonObjectRequest;

    if (jsonObjectRequest["type"] == "jobs")
    {
        // get jobs and send them back
        QJsonObject response;
        response["type"] = "jobs";

//        std::vector<Job> jobs = emit jobsRequest();
//        QJsonArray jobsArray;
//        for (auto job : jobs)
//        {
//            job.
//            jobsArray.push_back()
//        }

        this->SendToClient(response);
    }
    else if (jsonObjectRequest["type"] == "submit")
    {
        // just save that
        this->onFeedback("FEEDBACKK RAY2");
    }
}

void ConnectionThread::onFeedback(std::string feedback)
{
    // send feedback via conn socket
    QJsonObject response;
    response["type"] = "feedback";
    response["feedback"] = QJsonValue(QString::fromStdString(feedback));

    this->SendToClient(response);
}

void ConnectionThread::onDisconnected()
{
    // free the conn socket and terminate the thread
    this->connSocket->deleteLater();
    this->exit(0);
}
