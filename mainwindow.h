#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <candidatesoftware.h>
#include <network.h>
#include <QObject>
#include <job.h>

#include <QtUiTools/QUiLoader>
#include <QStackedWidget>
#include <ui_jobsWidget.h>
#include <ui_candidateWidget.h>
#include <ui_feedbackWidget.h>

namespace Ui {
class jobsForm;
class candidateWidget;
class feedbackWidget;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::jobsForm *uiJobsForm;
    Ui::candidateWidget *uiCandidateForm;
    Ui::feedbackWidget *uiFeedbackForm;

    QWidget *jobsWidget;
    QWidget *candidateWidget;
    QWidget *feedbackWidget;
    QStackedWidget *stackWidget;

    QComboBox *jobsComboBox;
    QTextEdit *jobsRequirementsTextEdit;
    QPushButton *jobsApplyBtn;
    QPushButton *candidateSubmitBtn;

    CandidateSoftware *client;
    std::vector<Job> jobs;
    std::string feedback;

    void initUiForms();
    void initStackedWidgets();
    void initNetworkWithServer();

public slots:
    void onJobReceived(std::vector<Job>);
    void onFeedbackReceived(std::string);

    void onJobSelected(int);
    void onApplyPressed();
    void onSubmit();

//signals:
};

#endif // MAINWINDOW_H
