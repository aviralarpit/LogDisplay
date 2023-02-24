/*
******************************************************************************
* License Information: Accord Global Technology Solutions Private Limited.
* 72 & 73, Krishna Reddy Colony, Domlur layout
* Bangalore - 560071, INDIA
* Licensed software and all rights reserved.
******************************************************************************
* File:                 LogDisplay.h
*
* Description:          Header file for logDisplay
*
* Compiler:
*
* Compiler options:
*
* Pragma:               none
*
* H/W platform:
*
* Portability:
*
* Author(s):            Arpit K @ AGTSPL
*
* Classes:              LogDisplay
*
* References:
*
* Version History:
* <Version Number> <Author> <date> <defect Number> <Modification made
* and reason
* for modification>
* 1.1
*****************************************************************************/
#ifndef LOGDISPLAY_H
#define LOGDISPLAY_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QCursor>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QToolBar>
#include <QPushButton>
#include <QtCore/QDateTime>
#include <algorithm>
#include <QDateTime>
#include<QMainWindow>
class LogDisplay : public QMainWindow
{
    Q_OBJECT

public:
    LogDisplay(QWidget *parent = nullptr);
    void addLog(const QString &message);
    void addToDisplay(const QString &logMessage);
    void clearDisplay();

    ~LogDisplay();

private slots:
    void updateLogDisplay();
    void fileChanged(const QString &path);
    void setMaxLines(int);

private:
    bool updateLogList();

    QPlainTextEdit *logDisplay;
    QCheckBox *errorCheckbox;
    QCheckBox *warningCheckbox;
    QCheckBox *infoCheckbox;
    QFileSystemWatcher *watcher;
    QStringList logList;
    int maxLines=10;
    QPushButton *clearButton;




};


#endif // LOGDISPLAY_H
