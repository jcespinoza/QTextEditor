#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.setApplicationName("QTextEditor");
    a.setApplicationVersion("0.0.1");
    a.setOrganizationName("Progra III Inc.");
    a.setOrganizationDomain("unitec.edu");
    QIcon icon(":/icons/jcicon.png");
    a.setWindowIcon(icon);
    return a.exec();
}
