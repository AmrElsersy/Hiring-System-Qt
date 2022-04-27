#include "company_mainwindow.h"

company_mainwindow::company_mainwindow(QWidget *parent) : QMainWindow(parent)
{
    this->server = new NetworkServer(8080, "/home/amrelsersy/HiringSystem/data");

    // jobs form
    this->jobsWidget = new QWidget();
    this->initJobsWidget();

    // create job form
    this->createJobWidget = new QWidget();
    this->initCreateJobWidget();

    // create applications widget
    this->applicationsWidget = new QWidget();

    // stack widget to switch widgets
    this->stackWidget = new QStackedWidget(this);
    this->stackWidget->addWidget(this->jobsWidget);
    this->stackWidget->addWidget(this->createJobWidget);
    this->stackWidget->addWidget(this->applicationsWidget);
    this->stackWidget->setCurrentIndex(0);

    this->setCentralWidget(this->stackWidget);
    this->resize(455, 530);
}

void company_mainwindow::onAddPressed()
{
    // create job widget
    this->stackWidget->setCurrentIndex(1);
}

void company_mainwindow::onCreateJobPressed()
{
    auto nameLineEdit = this->createJobWidget->findChild<QLineEdit*>("lineEdit");
    auto reqTextEdit = this->createJobWidget->findChild<QTextEdit*>("textEdit");

    std::string jobName = nameLineEdit->text().toStdString();
    std::string requirements = reqTextEdit->toPlainText().toStdString();

    Job job(jobName, requirements);

    // add the job to the server database
    this->server->AddJob(job);

    std::cout << "Job created successfully " << jobName << std::endl;
    // back to jobs widget
    this->stackWidget->setCurrentIndex(0);
}

void company_mainwindow::onFeedbackPressed(Candidate _c, std::string _feedback)
{
    auto candidates = this->server->GetCandidates();
    for (uint i = 0; i < candidates.size(); i++)
    {
        auto c = candidates[i];
        if (c.GetAge() == _c.GetAge() &&
            c.GetGPA() == _c.GetGPA() &&
            c.GetName() == _c.GetName() &&
            c.GetGender() == _c.GetGender() &&
            c.GetUniversity() == _c.GetUniversity() &&
            c.GetAppliedJob() == _c.GetAppliedJob())
        {
            candidates[i].SetFeedback(_feedback);
            this->server->SaveCandidates(candidates);
            std::cout << "Feedback Set to the server with {" << _feedback << "}" << std::endl;
            break;
        }
    }
    this->stackWidget->setCurrentIndex(0);
}

void company_mainwindow::onBackBtnPressed()
{
    this->stackWidget->setCurrentIndex(0);
}

void company_mainwindow::onShowPressed(std::string jobName)
{
    auto candidates = this->server->GetCandidates();

    // UI layout for all applications widgets
    QVBoxLayout *layout = new QVBoxLayout();

    for (auto candidate : candidates)
    {
        if (candidate.GetAppliedJob() != jobName)
            continue;

        auto applicationWidget = new ApplicationWidget(candidate);
        applicationWidget->setMinimumSize(450,450);
        connect(applicationWidget, SIGNAL(feedbackClicked(Candidate, std::string)), this, SLOT(onFeedbackPressed(Candidate, std::string)));

        layout->addWidget(applicationWidget);
    }
    QPushButton *exitBtn = new QPushButton("Back");
    layout->addWidget(exitBtn);
    connect(exitBtn, SIGNAL(clicked()), this, SLOT(onBackBtnPressed()));

    // add new application widget
    this->stackWidget->removeWidget(this->applicationsWidget);
    this->applicationsWidget = new QWidget();
    this->applicationsWidget->setLayout(layout);
    this->applicationsWidget->setStyleSheet("background-color:rgb(255,255,255);");

    // make the applications scrollable
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(this->applicationsWidget);
    this->stackWidget->addWidget(scrollArea);

    this->stackWidget->setCurrentIndex(2);
}

void company_mainwindow::initJobsWidget()
{
    this->uiJobsForm = new Ui::jobForm();
    this->uiJobsForm->setupUi(this->jobsWidget);

   auto jobsAddBtn = this->jobsWidget->findChild<QPushButton*>("pushButton");
    connect(jobsAddBtn, SIGNAL(clicked()), this, SLOT(onAddPressed()));

    QVBoxLayout *vlayout = this->jobsWidget->findChild<QVBoxLayout*>("verticalLayout");

    auto jobs = this->server->GetJobs();
    for (auto job: jobs)
    {
        auto jobWidget = new JobWidget(job.GetName(), this);
        vlayout->addWidget(jobWidget);
        connect(jobWidget, SIGNAL(clicked(std::string)), this, SLOT(onShowPressed(std::string)));
    }
}

void company_mainwindow::initCreateJobWidget()
{
    this->uiCreateJobForm = new Ui::createJobForm();
    this->uiCreateJobForm->setupUi(this->createJobWidget);

    auto createBtn = this->createJobWidget->findChild<QPushButton*>("pushButton");
    connect(createBtn, SIGNAL(clicked()), this, SLOT(onCreateJobPressed()));
}

// ============================== Application Widget ==============================
ApplicationWidget::ApplicationWidget(Candidate candidate, QWidget *parent): QWidget (parent)
{
    this->candidate = candidate;

    Ui::applicationForm *form = new Ui::applicationForm();
    form->setupUi(this);

    auto nameLabel = this->findChild<QLabel*>("nameLabel");
    nameLabel->setText(QString::fromStdString(candidate.GetName()));

    this->findChild<QLineEdit*>("age")->setText(QString::number(candidate.GetAge()));
    this->findChild<QLineEdit*>("gpa")->setText(QString::number(candidate.GetGPA()));
    this->findChild<QLineEdit*>("university")->setText(QString::fromStdString(candidate.GetUniversity()));
    this->findChild<QLineEdit*>("gender")->setText(QString::fromStdString(candidate.GetGender()));
    this->findChild<QLineEdit*>("feedback")->setText(QString::fromStdString(candidate.GetFeedback()));

    auto awardsTextEdit = this->findChild<QTextEdit*>("awards");
    QString awardsText;
    for (auto award : candidate.GetAwards())
    {
        award += "\n";
        awardsText.append(QString::fromStdString(award));
    }
    awardsTextEdit->setText(awardsText);

    // signals & slots
    auto setFeedbackBtn = this->findChild<QPushButton*>("pushButton");
    this->feedbackLineEdit = this->findChild<QLineEdit*>("feedback");
    connect(setFeedbackBtn, SIGNAL(clicked()), this, SLOT(onClicked()));
}

void ApplicationWidget::onClicked()
{
    emit feedbackClicked(this->candidate, this->feedbackLineEdit->text().toStdString());
}

// ============================== Job Widget ==============================
JobWidget::JobWidget(std::string jobName, QWidget *parent): QWidget (parent)
{
    this->hJobLayout = new QHBoxLayout();
    this->hJobLayout->setSpacing(10);

    this->jobNameLabel = new QLabel(QString::fromStdString(jobName));
    this->showJobBtn = new QPushButton("Show");
    connect(this->showJobBtn, SIGNAL(clicked()), this, SLOT(onClicked()));

    this->hJobLayout->setAlignment(this->jobNameLabel, Qt::AlignLeft);
    this->hJobLayout->setAlignment(this->showJobBtn, Qt::AlignRight);

    this->hJobLayout->addWidget(this->jobNameLabel);
    this->hJobLayout->addWidget(this->showJobBtn);

    this->setLayout(hJobLayout);
}

void JobWidget::onClicked()
{
    emit clicked(this->jobNameLabel->text().toStdString());
}
