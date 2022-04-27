#ifndef COMPANY_MAINWINDOW_H
#define COMPANY_MAINWINDOW_H

#include <QMainWindow>
#include <server.h>
#include <QStackedWidget>
#include <ui_jobs_company.h>
#include <ui_create_job_company.h>
#include <ui_jobApplication.h>

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QScrollArea>
#include <QtCharts>

namespace Ui {
class jobForm;
class createJobForm;
class applicationForm;
}

class JobWidget: public QWidget
{
    Q_OBJECT
public:
    JobWidget(std::string name, QWidget *parent = nullptr);
private:
    QHBoxLayout *hJobLayout;
    QLabel *jobNameLabel;
    QPushButton *showJobBtn;

public slots:
    void onClicked();
signals:
    void clicked(std::string);
};

class ApplicationWidget: public QWidget
{
    Q_OBJECT
public:
    ApplicationWidget(Candidate candidate, QWidget *parent = nullptr);

private:
    Candidate candidate;
    QLineEdit *feedbackLineEdit;

signals:
    void feedbackClicked(Candidate, std::string);
public slots:
    void onClicked();

};

class company_mainwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit company_mainwindow(QWidget *parent = nullptr);

signals:

public slots:
    void onShowPressed(std::string);
    void onAddPressed();
    void onCreateJobPressed();
    void onFeedbackPressed(Candidate, std::string);
    void onBackBtnPressed();
    void onStatisticsPressed();

private:
    NetworkServer *server;
    Ui::jobForm *uiJobsForm;
    Ui::createJobForm *uiCreateJobForm;

    QWidget *jobsWidget;
    QWidget *createJobWidget;
    QWidget *applicationsWidget;
    QStackedWidget *stackWidget;
    QScrollArea *scrollArea;

    void initJobsWidget();
    void initCreateJobWidget();

};

#endif // COMPANY_MAINWINDOW_H
