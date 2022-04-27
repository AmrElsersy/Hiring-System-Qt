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

signals:
    void SubmitRequest(QJsonObject, ConnectionThread*);
    QJsonObject RequestJobs();
    std::string RequestFeedback(ConnectionThread*);
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
    bool SaveJson(std::string path, QJsonObject object);

    /// \brief Convert all jobs in the server to json format to be saved
    /// \return Json object in format {"jobs": [{"name":"job1", "requirements":"req1"}]}
    QJsonObject SerializeJobs();

    /// \brief Convert all candidates in the server to json format to be saved
    /// \return Json object in format {"candidates": [{"name":"candidate1", ...}]}
    QJsonObject SerializeCandidates();

    void SaveCandidates(std::vector<Candidate>);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

public slots:
    void AddJob(Job job);
    void SubmitCandidate(QJsonObject, ConnectionThread*);
    QJsonObject RequestJobs();
    std::string RequestFeedback(ConnectionThread*);

private:
    std::vector<Job> jobs;
    std::vector<Candidate> candidates;
    std::string jobsJsonPath;
    std::string candidatesJsonPath;
    std::map<ConnectionThread *, Candidate> clinetConnections;

    void ReadJobs();
    void ReadCandidates();
};

#endif // SERVER_H
