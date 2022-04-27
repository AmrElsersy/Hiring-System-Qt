#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <network.h>
#include <QJsonObject>
#include <candidatesoftware.h>

#define JOBS_IDX 0
#define CANDIDATE_IDX 1
#define FEEDBACK_IDX 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(600, 650);

    this->jobsWidget = new QWidget();
    this->candidateWidget = new QWidget();
    this->feedbackWidget = new QWidget();

    this->initUiForms();
    this->initStackedWidgets();
    this->initNetworkWithServer();

    // Jobs Widget
    this->jobsRequirementsTextEdit = this->jobsWidget->findChild<QTextEdit*>("textEdit");
    this->jobsApplyBtn = this->jobsWidget->findChild<QPushButton*>("pushButton");
    this->jobsComboBox = this->jobsWidget->findChild<QComboBox*>("comboBox");
    connect(this->jobsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onJobSelected(int)));
    connect(this->jobsApplyBtn, SIGNAL(clicked()), this, SLOT(onApplyPressed()));

    // candidate application Widget
    this->candidateSubmitBtn = this->candidateWidget->findChild<QPushButton*>("submitButton");
    connect(this->candidateSubmitBtn, SIGNAL(clicked()), this, SLOT(onSubmit()));

    // application widget: gender combobox
    auto comboBoxGender = this->candidateWidget->findChild<QComboBox*>("comboBoxGender");
    comboBoxGender->addItem("Male");
    comboBoxGender->addItem("Female");

    // feedback widget: close btn, refresh btn
    auto closeBtn = this->feedbackWidget->findChild<QPushButton*>("pushButton");
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(onClose()));
    auto refreshBtn = this->feedbackWidget->findChild<QPushButton*>("pushButton_2");
    connect(refreshBtn , SIGNAL(clicked()), this, SLOT(onRequestFeedback()));
}

MainWindow::~MainWindow()
{
    delete this->uiJobsForm;
}

void MainWindow::initNetworkWithServer()
{
    std::string address = "127.0.0.1";
    quint16 port = 8080;

    this->client = new CandidateSoftware(address, port);
    connect(this->client, SIGNAL(jobsReceived(std::vector<Job>)), this, SLOT(onJobReceived(std::vector<Job>)));
    connect(this->client, SIGNAL(feedbackReceived(std::string)), this, SLOT(onFeedbackReceived(std::string)));

    this->client->RequestJobs();
}

void MainWindow::initUiForms()
{
    this->uiJobsForm = new Ui::jobsForm();
    this->uiJobsForm->setupUi(jobsWidget );

    this->uiCandidateForm = new Ui::candidateWidget();
    this->uiCandidateForm->setupUi(this->candidateWidget);

    this->uiFeedbackForm = new Ui::feedbackWidget();
    this->uiFeedbackForm->setupUi(this->feedbackWidget);
}

void MainWindow::initStackedWidgets()
{
    this->stackWidget = new QStackedWidget(this);
    this->setCentralWidget(stackWidget);

    this->stackWidget->insertWidget(JOBS_IDX, jobsWidget);
    this->stackWidget->insertWidget(CANDIDATE_IDX, candidateWidget);
    this->stackWidget->insertWidget(FEEDBACK_IDX, feedbackWidget);
    this->stackWidget->setCurrentIndex(JOBS_IDX);
}

void MainWindow::onJobReceived(std::vector<Job> _jobs)
{
    this->jobs = _jobs;

    // add jobs names to the combo box so that user choose the job he is applying to
    for (auto job : _jobs)
    {
        std::string name = job.GetName();
        this->jobsComboBox->addItem(QString::fromStdString(name));
    }
}

void MainWindow::onFeedbackReceived(std::string _feedback)
{
    this->feedback = _feedback;
    qDebug() << "received Feedback" << QString::fromStdString(_feedback);

    // set the gui's feedback text with the received text feedback
    auto feedbackTextEdit = this->feedbackWidget->findChild<QTextEdit*>("textEdit");
    feedbackTextEdit->setText(QString::fromStdString(feedback));
}

void MainWindow::onJobSelected(int index)
{
    std::string jobNameApplied = this->jobsComboBox->itemText(index).toStdString();
    for (auto job : this->jobs)
    {
        if (job.GetName() == jobNameApplied)
        {
            // change the textEdit with the selected job requirements
            std::string desc = job.GetDescription();
            this->jobsRequirementsTextEdit->setText(QString::fromStdString(desc));
            return;
        }
    }
}

void MainWindow::onApplyPressed()
{
    // set the applied job of the client to the selected job name
    std::string jobNameApplied = this->jobsComboBox->currentText().toStdString();
    this->client->GetCandidate()->SetAppliedJob(jobNameApplied);

    // change mainwindow to the candidate application window
    this->stackWidget->setCurrentIndex(CANDIDATE_IDX);
}

void MainWindow::onSubmit()
{
    // get the line edit widgets that the user filled
    auto lineEditName = this->candidateWidget->findChild<QLineEdit*>("lineEditName");
    auto lineEditAge = this->candidateWidget->findChild<QLineEdit*>("lineEditAge");
    auto lineEditGPA = this->candidateWidget->findChild<QLineEdit*>("lineEditGPA");
    auto lineEditMajor = this->candidateWidget->findChild<QLineEdit*>("lineEditMajor");
    auto lineEditUni = this->candidateWidget->findChild<QLineEdit*>("lineEditUni");
    auto textEditAwards = this->candidateWidget->findChild<QTextEdit*>("textEditAwards");
    auto comboBoxGender = this->candidateWidget->findChild<QComboBox*>("comboBoxGender");

    // set the candidate's application values
    auto candidate = this->client->GetCandidate();
    candidate->SetName(lineEditName->text().toStdString());
    candidate->SetAge(lineEditAge->text().toInt());
    candidate->SetGPA(lineEditGPA->text().toDouble());
    candidate->SetMajor(lineEditMajor->text().toStdString());
    candidate->SetUniversiry(lineEditUni->text().toStdString());
    candidate->SetGender(comboBoxGender->currentText().toStdString());

    QStringList lines = textEditAwards->toPlainText().split("\n");
    for (auto line : lines)
        candidate->AddAward(line.toStdString());

    // send the data to the server to submit it
    this->client->Submit();

    // change the main window to the feedback window
    this->stackWidget->setCurrentIndex(FEEDBACK_IDX);
    this->resize(400, 450);
}

void MainWindow::onClose()
{
    this->close();
}

void MainWindow::onRequestFeedback()
{
    this->client->RequestFeedback();
}

