#ifndef CANDIDATESOFTWARE_H
#define CANDIDATESOFTWARE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

#include <network.h>
#include <job.h>
#include <candidate.h>

#include <bits/stdc++.h>

class CandidateSoftware: public QObject
{
    Q_OBJECT
public:
    CandidateSoftware(std::string address, quint16 port);
    void Submit();
    Candidate *GetCandidate();
    void RequestJobs();

private:
    Candidate *candidate;
    NetworkClient *network;
    std::vector<Job> jobs;
    std::string feedback;

    void DeserializeJobs(QJsonObject);
    void DeserializeFeedback(QJsonObject);

public slots:
    void onNewMessage();
    void onConnected();

signals:
    void jobsReceived(std::vector<Job>);
    void feedbackReceived(std::string);
};

#endif // CANDIDATESOFTWARE_H
