#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>  //文件对话框
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hfmTree = 0;//先初始化为空指针

    /////////窗口设置////////
    setWindowTitle("文本压缩工具");

    ///////功能设置
    //添加文件
    connect(ui->btn_open,&QPushButton::clicked,[=](){
        path1 = QFileDialog::getOpenFileName(this,"打开文件","","文本(*.txt *.dat)");
        if(path1 != "")
        {
            QFile file(path1);
            if(file.open(QIODevice::ReadOnly))
            {
                if(path1.endsWith(".txt"))
                {
                    QByteArray array = file.readAll();
                    qDebug() << array;
                    ui->textEdit->setText( array );//展示打开的文本
                    hfmTree = new HuffmanTree(this, path1);//新建哈夫曼树
                }
                else if(path1.endsWith(".dat"))
                {
                    QDataStream ds(&file);
                    ds.setVersion(QDataStream::Qt_5_9);
                    ds.setByteOrder(QDataStream::LittleEndian);
                    QString str;
                    ds >> str;
                    qDebug() << str;
                    ui->textEdit->setText( str );
                    hfmTree = new HuffmanTree(this);//新建哈夫曼树
                }
                file.close();
            }
            hfmTree->print(ui->label_print);
        }



    });


    //压缩
    connect(ui->btn_enCoding,&QPushButton::clicked,[=](){
        QString path2 = QFileDialog::getSaveFileName(this, "读取待压缩文件", "", "二进制文件(*.dat)");
        if(path2 != "")
        {
            hfmTree->enCoding(path1, path2, ui->textEdit);
            path1 = path2;
        }
    });

    //解压
    connect(ui->btn_deCoding,&QPushButton::clicked,[=](){
        QString path2 = QFileDialog::getSaveFileName(this, "保存文件", "", "文本(*.txt)");
        hfmTree->deCoding(path1, path2, ui->textEdit);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
