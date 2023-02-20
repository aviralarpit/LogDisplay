#include "logdisplay.h"
#include<QLabel>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
#include <QSettings>

LogDisplay::LogDisplay(QWidget *parent)
    : QMainWindow(parent)
{
    logDisplay = new QPlainTextEdit(this);
    setCentralWidget(logDisplay);

    QToolBar *toolBar = addToolBar("Filters");

    errorCheckbox = new QCheckBox("ERROR", this);
    errorCheckbox->setStyleSheet("color: red;");
    errorCheckbox->setFont(QFont("Helvetica",8, QFont::Bold));
    //errorCheckbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    warningCheckbox = new QCheckBox("WARNING", this);
    warningCheckbox->setStyleSheet("color: orange;");
    warningCheckbox->setFont(QFont("Helvetica",8, QFont::Bold));
    //warningCheckbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    infoCheckbox = new QCheckBox("INFO", this);
    infoCheckbox->setFont(QFont("Helvetica",8, QFont::Bold));
    infoCheckbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    toolBar->addWidget(errorCheckbox);
    toolBar->addWidget(warningCheckbox);
    toolBar->addWidget(infoCheckbox);
    toolBar->addSeparator();

    QLabel *textLabel = new QLabel("Show log lines:");
    textLabel->setFont(QFont("Helvetica",8, QFont::Bold));
    toolBar->addWidget(textLabel);

    QPushButton *tenLinesButton = new QPushButton("10 Lines");
    tenLinesButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QPushButton *twentyLinesButton = new QPushButton("20 Lines");
    twentyLinesButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QPushButton *hundredLinesButton = new QPushButton("100 Lines");
    hundredLinesButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    clearButton = new QPushButton("Clear Logs", this);
    clearButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toolBar->addWidget(tenLinesButton);
    toolBar->addWidget(twentyLinesButton);
    toolBar->addWidget(hundredLinesButton);
    toolBar->addSeparator();
    toolBar->addWidget(clearButton);
    toolBar->addSeparator();

    connect(errorCheckbox, &QCheckBox::stateChanged, this, &LogDisplay::updateLogDisplay);
    connect(warningCheckbox, &QCheckBox::stateChanged, this, &LogDisplay::updateLogDisplay);
    connect(infoCheckbox, &QCheckBox::stateChanged, this, &LogDisplay::updateLogDisplay);
    connect(tenLinesButton, &QPushButton::clicked, [this]() { setMaxLines(10); });
    connect(twentyLinesButton, &QPushButton::clicked, [this]() { setMaxLines(20); });
    connect(hundredLinesButton, &QPushButton::clicked, [this]() { setMaxLines(100); });
    connect(clearButton, &QPushButton::clicked, this, &LogDisplay::clearDisplay);
    infoCheckbox->setChecked(true);
////New implementation for reading file
        QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
            QString logFilePath = settings.value("logFilePath").toString();

//            QFile file(logFilePath);
//            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//                return;



///// Old working

    watcher = new QFileSystemWatcher(this);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &LogDisplay::fileChanged);
    watcher->addPath(logFilePath);

    if (!updateLogList()) {
        QMessageBox::critical(this, "Error", "Failed to read log file.");
    }
    updateLogDisplay();
}

LogDisplay::~LogDisplay()
{
}
void LogDisplay::setMaxLines(int maxLines)
{
    this->maxLines = maxLines;
    updateLogDisplay();
}

////NEW
/* It first clears the logList object to make sure there are no pre-existing logs in it. It then creates a QFile object with the path of the log file to be read.
If the file is unable to be opened in read-only and text mode, the function returns false to indicate failure.
If the file is successfully opened, it reads each line from the file using a QTextStream object and appends it to the logList object.
Once all lines have been read and appended, the function closes the file and returns true to indicate success. */
bool LogDisplay::updateLogList()
{
    logList.clear();
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    QString logFilePath = settings.value("logFilePath").toString();


    QFile file(logFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        logList.append(in.readLine());
    }
    file.close();
    return true;
}
/*
This function updates the display of the logs. It first clears the log display area using the clear() function.
It then creates a QTextCursor object and moves it to the end of the document, which will be the location where the new logs will be inserted.
The function then loops through the logList object in reverse order, starting from the latest log.
If the count of inserted lines exceeds the maxLines value, the loop breaks. For each log, the function checks if the error, warning, or info checkboxes are checked and whether the log contains the corresponding keyword.
If the log meets these conditions, it formats the text with a different color for errors and warnings, and inserts it into the log display area using the QTextCursor object. The cursor is then moved up one line, and the formatting and insertion process is repeated for the next log until either the maximum number of lines is reached or all logs in the logList object have been checked.
*/
void LogDisplay::updateLogDisplay()
{
    logDisplay->clear();
    int count = 0;

    QTextCursor cursor(logDisplay->document());
    cursor.movePosition(QTextCursor::End);

    for(int i = logList.size() - 1; i >= 0 && count < maxLines; --i)
    {
        QString log = logList[i];

        if ((errorCheckbox->isChecked() && log.contains("ERROR")) ||
            (warningCheckbox->isChecked() && log.contains("WARNING")) ||
            (infoCheckbox->isChecked() && log.contains("INFO"))) {

            QTextCharFormat format;
            if (log.contains("ERROR")) {
                format.setForeground(Qt::red);
            } else if (log.contains("WARNING")) {
                format.setForeground(Qt::darkYellow);
            }

            cursor.insertText("\n");
            cursor.movePosition(QTextCursor::Up);
            cursor.setCharFormat(format);
            cursor.insertText(log);
            ++count;
        }
    }
}


void LogDisplay::fileChanged(const QString &path)
   {
      Q_UNUSED(path)
      if (!updateLogList()) {
      QMessageBox::critical(this, "Error", "Failed to read log file.");
      return;
      }
      updateLogDisplay();
    }
void LogDisplay::addToDisplay(const QString &logMessage)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeStamp = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    logDisplay->appendPlainText(timeStamp + " " + logMessage);
}

void LogDisplay::clearDisplay()
{
    logDisplay->clear();
}
