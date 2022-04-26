#include "company_mainwindow.h"

company_mainwindow::company_mainwindow(QWidget *parent) : QMainWindow(parent)
{
    this->server = new NetworkServer(8080, "/home/amrelsersy/HiringSystem/data");
}
