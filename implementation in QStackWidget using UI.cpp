#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logdisplay.h"
#include "QHBoxLayout"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LogDisplay *logDisplay = new LogDisplay();
    ui->page->setLayout(new QHBoxLayout);
    ui->page->layout()->addWidget(logDisplay);
}

MainWindow::~MainWindow()
{
    delete ui;
}
