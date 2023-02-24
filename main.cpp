/*
******************************************************************************
* License Information: Accord Global Technology Solutions Private Limited.
* 72 & 73, Krishna Reddy Colony, Domlur layout
* Bangalore - 560071, INDIA
* Licensed software and all rights reserved.
******************************************************************************
* File:                 main.cpp
*
* Description:          QT main class
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
#include "logdisplay.h"
#include<QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogDisplay logDisplay;
    logDisplay.show();


    return a.exec();
}
