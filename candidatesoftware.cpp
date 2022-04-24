#include "candidatesoftware.h"

CandidateSoftware::CandidateSoftware(std::string address, quint16 port)
{
    this->candidate = new Candidate();
    this->network = new NetworkClient(address, port);

    connect(this->network->GetSocket(), SIGNAL(readyRead()), this, SLOT(onNewMessage()));
    connect(this->network->GetSocket(), SIGNAL(connected()), this, SLOT(onConnected()));

    this->network->ConnectToServer();
}

void CandidateSoftware::RequestJobs()
{
    QJsonObject requestObject;
    requestObject["type"] = "jobs";
    this->network->Send(requestObject);
}

void CandidateSoftware::Submit()
{
    QJsonObject candidateJson = candidate->SerializeToJson();
    candidateJson["type"] = "submit";

    this->network->Send(candidateJson);
}

Candidate *CandidateSoftware::GetCandidate()
{
    return this->candidate;
}

void CandidateSoftware::DeserializeJobs(QJsonObject response)
{
    /*
     * {
     *  "type": "jobs"
     *  "jobs": [
     *              {
     *                  "name": "MED Software Developer",
     *                  "requirements" : "C++ OOP"
     *              }
     *              {
     *                  "name": "Computer Vision",
     *                  "requirements" : "Python Tensorflow"
     *              }
     *          ]
     * }
     */

    QJsonValue value = response["jobs"];
    if (!value.isArray())
    {
        qDebug() << "error in response format ";
        return;
    }
    QJsonArray jobsArray = value.toArray();
    for (int i = 0; i < jobsArray.size(); i++)
    {
        auto jobJson = jobsArray[i].toObject();

        std::string jobName = jobJson["name"].toString().toStdString();
        std::string jobRequirments = jobJson["requirements"].toString().toStdString();
        Job job(jobName, jobRequirments);
        this->jobs.push_back(job);
    }
}

void CandidateSoftware::DeserializeFeedback(QJsonObject response)
{
    this->feedback = response["feedback"].toString().toStdString();
}

void CandidateSoftware::onNewMessage()
{
    QJsonObject response = this->network->Receive();
    if (response["type"] == "jobs")
    {
        this->DeserializeJobs(response);
        emit jobsReceived(this->jobs);
    }
    else if (response["type"] == "feedback")
    {
        this->DeserializeFeedback(response);
        emit feedbackReceived(this->feedback);
    }
}

void CandidateSoftware::onConnected()
{
}
