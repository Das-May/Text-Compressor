#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "huffmantree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString path1;//记录用户所打开的文件的路径
    HuffmanTree *hfmTree;
};

#endif // MAINWINDOW_H
