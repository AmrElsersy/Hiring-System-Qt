#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <candidatesoftware.h>
#include <network.h>
#include <QObject>
#include <job.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    CandidateSoftware *client;
    std::vector<Job> jobs;
    std::string feedback;

public slots:
    void onJobReceived(std::vector<Job>);
    void onFeedbackReceived(std::string);
//signals:
};

#endif // MAINWINDOW_H
