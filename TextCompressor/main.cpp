#include "mainwindow.h"
#include <QApplication>
#include "huffmantree.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ////////测试
//    HuffmanTree humTree;
//    humTree.print();

    return a.exec();
}
