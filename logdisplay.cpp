#include "logdisplay.h"
#include<QLabel>
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

    watcher = new QFileSystemWatcher(this);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &LogDisplay::fileChanged);
    watcher->addPath("C:/Users/arpit.k/Downloads/LogTail/logtail/sample.txt");

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
//Working
//bool LogDisplay::updateLogList()
//{
//    logList.clear();
//    QFile file("C:/Users/arpit.k/Downloads/LogTail/logtail/sample.txt");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        return false;
//    }
//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        logList.append(in.readLine());
//    }
//    file.close();
//    return true;
//   }
//////Working
//void LogDisplay::updateLogDisplay()
//{
//    logDisplay->clear();
//    int count = 0;

//    QTextCursor cursor(logDisplay->document());
//    cursor.movePosition(QTextCursor::End);



//        for(int i = logList.size() - 1; i >= 0 && count < maxLines; --i)
//        {
//            QString log = logList[i];

//            if ((errorCheckbox->isChecked() && log.contains("ERROR")) ||
//                (warningCheckbox->isChecked() && log.contains("WARNING")) ||
//                (infoCheckbox->isChecked() && log.contains("INFO"))) {

//                QTextCharFormat format;
//                if (log.contains("ERROR")) {
//                    format.setForeground(Qt::red);
//                } else if (log.contains("WARNING")) {
//                    format.setForeground(Qt::darkYellow);
//                }

//                cursor.setCharFormat(format);
//                cursor.insertText(log + "\n");
//                ++count;
//            }
//        }

//}

////NEW
bool LogDisplay::updateLogList()
{
    logList.clear();
    QFile file("C:/Users/arpit.k/Downloads/LogTail/logtail/sample.txt");
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
