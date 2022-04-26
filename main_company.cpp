#include "company_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    company_mainwindow *w = new company_mainwindow();
    w->show();

    return a.exec();
}
