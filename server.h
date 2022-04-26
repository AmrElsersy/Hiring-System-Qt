#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QHostAddress>
#include <QAbstractSocket>

#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include <QObject>

#include <job.h>
#include <candidate.h>

// Thread for handling connection with the server
class ConnectionThread: public QThread
{
    Q_OBJECT
public:
    explicit ConnectionThread(qintptr socketDescriptor, QObject *parent = nullptr);
    void run();
    void SendToClient(QJsonObject);

private:
    QTcpSocket *connSocket;
    qintptr socketDescriptor;

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
    explicit NetworkServer(quint16 port,std::string jsonPath, QObject *parent = nullptr);
    std::vector<Job> GetJobs();
    std::vector<Candidate> GetCandidates();
    void SetJsonPath(std::string path);

    QJsonObject ReadJson(std::string path);
    bool SaveJson(std::string path, QJsonObject);

    QJsonObject SerializeJobs();
    QJsonObject SerializeCandidates();

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

public slots:
    void AddJob(Job job);
    void SubmitCandidate(Candidate candidate);
    void SendFeedback(std::string feedback);

private:
    std::vector<Job> jobs;
    std::vector<Candidate> candidates;
    std::string jobsJsonPath;
    std::string candidatesJsonPath;

    void ReadJobs();
    void ReadCandidates();
};

#endif // SERVER_H
