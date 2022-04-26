#ifndef COMPANY_MAINWINDOW_H
#define COMPANY_MAINWINDOW_H

#include <QMainWindow>
#include <server.h>

class company_mainwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit company_mainwindow(QWidget *parent = nullptr);

signals:

public slots:

private:
    NetworkServer *server;
};

#endif // COMPANY_MAINWINDOW_H
