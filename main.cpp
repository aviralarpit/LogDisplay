#include "logdisplay.h"
#include<QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogDisplay logDisplay;
    logDisplay.show();


    return a.exec();
}
