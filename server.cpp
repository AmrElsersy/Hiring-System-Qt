#include "server.h"

NetworkServer::NetworkServer(quint16 port, std::string jsonPath, QObject *parent) :QTcpServer(parent)
{
    // initlaize server and listen
    if(this->listen(QHostAddress("127.0.0.1"), port))
        qDebug() << "Server is listening on port " << port;
    else
        qDebug() << "Failed to initiate server on port " << port;

    this->SetJsonPath(jsonPath);
    this->ReadJobs();
    this->ReadCandidates();
}

std::vector<Job> NetworkServer::GetJobs()
{
    return this->jobs;
}

std::vector<Candidate> NetworkServer::GetCandidates()
{
    return this->candidates;
}

void NetworkServer::SetJsonPath(std::string path)
{
    // if path conatins / at the end, delete it
    if (path[path.size()-1] == '/')
        path.erase(path.size()-1, 1);

    this->jobsJsonPath = path + "/jobs.json";
    this->candidatesJsonPath = path + "/candidates.json";
}

QJsonObject NetworkServer::ReadJson(std::string path)
{
    if (!QFileInfo::exists(QString::fromStdString(path)))
        return QJsonObject();

    QFile file(QString::fromStdString(path));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString text = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject object = document.object();
    return object;
}

bool NetworkServer::SaveJson(std::string path, QJsonObject object)
{
    QFile saveFile(QString::fromStdString(path));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument saveDoc(object);
    saveFile.write(saveDoc.toJson());
    return true;
}

QJsonObject NetworkServer::SerializeJobs()
{
    QJsonArray jobsArray;
    for (auto job : this->jobs)
    {
        QJsonObject jobJson = job.SerializeToJson();
        jobsArray.push_back(jobJson);
    }

    QJsonObject object;
    object["jobs"] = jobsArray;
    return object;
}

QJsonObject NetworkServer::SerializeCandidates()
{
    QJsonArray candidatesArray;
    for (auto candidate: this->candidates)
    {
        QJsonObject candidateJson = candidate.SerializeToJson();
        candidatesArray.push_back(candidateJson);
    }

    QJsonObject candidatesJson;
    candidatesJson["candidates"] = candidatesArray;
    return candidatesJson;
}

void NetworkServer::incomingConnection(qintptr socketDescriptor)
{
    // create thread and set its socket descriptor (for conn communication)
    ConnectionThread *thread = new ConnectionThread(socketDescriptor);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // connect thread with outside world
    connect(thread, SIGNAL(SubmitRequest(QJsonObject,ConnectionThread*)), this, SLOT(SubmitCandidate(QJsonObject,ConnectionThread*)));
    connect(thread, SIGNAL(RequestJobs()), this, SLOT(RequestJobs()));
    connect(thread, SIGNAL(RequestFeedback(ConnectionThread*)), this, SLOT(RequestFeedback(ConnectionThread*)));

    // start thread (will execute thread.run())
    thread->start();
}

void NetworkServer::AddJob(Job job)
{
    this->jobs.push_back(job);
    QJsonObject jobsObject = this->SerializeJobs();
    this->SaveJson(this->jobsJsonPath, jobsObject);
}

void NetworkServer::SaveCandidates(std::vector<Candidate> _candidates)
{
    this->candidates = _candidates;
    QJsonObject candidatesObject = this->SerializeCandidates();
    this->SaveJson(this->candidatesJsonPath, candidatesObject);
}

QJsonObject NetworkServer::RequestJobs()
{
    return this->SerializeJobs();
}

std::string NetworkServer::RequestFeedback(ConnectionThread * clientConn)
{
    return this->clinetConnections[clientConn].GetFeedback();
}

void NetworkServer::SubmitCandidate(QJsonObject candidateObject, ConnectionThread* candidateThread)
{
    Candidate candidate;
    candidate.SetFromJson(candidateObject);
    this->candidates.push_back(candidate);

    // save json object contains all the candidates
    QJsonObject candidatesObject = this->SerializeCandidates();
    this->SaveJson(this->candidatesJsonPath, candidatesObject);

    // save the thread that called the submit function to use that to send back a feedback
    this->clinetConnections[candidateThread] = candidate;
}

void NetworkServer::ReadJobs()
{
    QJsonObject object = this->ReadJson(this->jobsJsonPath);
    if (!object.contains("jobs"))
    {
        qDebug() << "Saving for first time " << QString::fromStdString(this->jobsJsonPath);
        // save empty json into the file
        this->SaveJson(this->jobsJsonPath, object);
        return;
    }

    QJsonArray jobsArray = object["jobs"].toArray();

    this->jobs.clear();
    for (auto jobValue : jobsArray)
    {
        QJsonObject jobObject = jobValue.toObject();
        Job job(jobObject);
        this->jobs.push_back(job);
    }
}

void NetworkServer::ReadCandidates()
{
    QJsonObject object = this->ReadJson(this->candidatesJsonPath);
    if (!object.contains("candidates"))
    {
        qDebug() << "Saving for first time " << QString::fromStdString(this->candidatesJsonPath);
        // save empty json into the file
        this->SaveJson(this->candidatesJsonPath, object);
        return;
    }

    QJsonArray candidatesArray = object["candidates"].toArray();

    this->candidates.clear();
    for (auto candidateValue : candidatesArray)
    {
        QJsonObject candidateObject = candidateValue.toObject();
        Candidate candidate;
        candidate.SetFromJson(candidateObject);
        this->candidates.push_back(candidate);
    }
}

//////////////////////////////////////////////////////////////////////////////

ConnectionThread::ConnectionThread(qintptr socketDescriptor, QObject *parent): QThread (parent)
{
    this->socketDescriptor = socketDescriptor;
}

void ConnectionThread::run()
{
    // assign the socket to the socket id descriptor
    this->connSocket = new QTcpSocket();
    this->connSocket->setSocketDescriptor(this->socketDescriptor);

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

        qDebug() << "Thread.onReadyRead" << jsonObjectRequest;

        QJsonObject jobsObject = emit RequestJobs();
        qDebug() << "emit result jobs " << jobsObject;

        response["jobs"] = jobsObject["jobs"];

        // reply in format {type: jobs, jobs:[{job1}, {job2}] }
        this->SendToClient(response);
    }
    else if (jsonObjectRequest["type"] == "submit")
    {
        // just save the candidate application info to the server
        emit SubmitRequest(jsonObjectRequest["candidate"].toObject(), this);
    }
    else if (jsonObjectRequest["type"] == "feedback")
    {
        std::string feedback = emit RequestFeedback(this);

        // send feedback via conn socket
        QJsonObject response;
        response["type"] = "feedback";
        response["feedback"] = QJsonValue(QString::fromStdString(feedback));

        this->SendToClient(response);
    }
}

void ConnectionThread::onDisconnected()
{
    // free the conn socket and terminate the thread
    this->connSocket->deleteLater();
    this->exit(0);
}
