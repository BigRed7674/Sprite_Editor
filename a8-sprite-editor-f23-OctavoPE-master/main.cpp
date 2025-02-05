//Reviewed By Travis Hobbs using Team Team Code Style Document
#include "mainwindow.h"
#include <QApplication>

/*
 * Main does the basic setup for the Application and initializes the MainWindow class.
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
