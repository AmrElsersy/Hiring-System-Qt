#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <network.h>
#include <QJsonObject>
#include <candidatesoftware.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string address = "127.0.0.1";
    quint16 port = 8080;

    this->client = new CandidateSoftware(address, port);
    connect(this->client, SIGNAL(jobsReceived(std::vector<Job>)), this, SLOT(onJobReceived(std::vector<Job>)));
    connect(this->client, SIGNAL(feedbackReceived(std::string)), this, SLOT(onFeedbackReceived(std::string)));

    this->client->RequestJobs();

    // GUI will set that
    Candidate *candidate = this->client->GetCandidate();
    candidate->SetName("Amr Elsersy");
    candidate->SetAge(24);
    candidate->SetGPA(3.1);
    candidate->SetMajor("Computer Engineering");
    candidate->SetUniversiry("ASU");
    candidate->SetAppliedJob("MED Software Engineer");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onJobReceived(std::vector<Job> _jobs)
{
    this->jobs = _jobs;

    qDebug() << "Received Jobs";
    for (auto job : _jobs)
        qDebug() << QString::fromStdString(job.GetName());

    this->client->Submit();
}

void MainWindow::onFeedbackReceived(std::string _feedback)
{
    this->feedback = _feedback;
    qDebug() << "reveived Feeeedbacccckkkkkk" << QString::fromStdString(_feedback);

}

