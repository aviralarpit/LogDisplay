/*
******************************************************************************
* License Information: Accord Global Technology Solutions Private Limited.
* 72 & 73, Krishna Reddy Colony, Domlur layout
* Bangalore - 560071, INDIA
* Licensed software and all rights reserved.
******************************************************************************
* File:             Example for Implementing it into UI application
*
* Description:
*
* Compiler:
*
* Compiler options:
*
* Pragma:
*
* H/W platform:
*
* Portability:
*
* Author(s):            Arpit K @ AGTSPL
*
* Classes:
*
* References:
*
* Version History:
* <Version Number> <Author> <date> <defect Number> <Modification made
* and reason
* for modification>
* 1.1
*****************************************************************************/
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
