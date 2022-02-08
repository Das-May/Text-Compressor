#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <QObject>
#include <QLabel>
#include <QTextEdit>
#include <QString>
#include <QByteArray>
#include "minheap.h"

struct node
{
    char ch;
    int index;//记录索引号，辨别压入堆中之后的结点
    int weight;//记录权值
    int parent,child[2];//双亲 左孩子 右孩子
    bool operator <(const node& a) const{
        return (weight < a.weight);
    }
};

class HuffmanTree : public QObject
{
    Q_OBJECT
public:
    explicit HuffmanTree(QObject *parent = nullptr, QString path = "");

    void print(QLabel *label = nullptr);//打印

    void enCoding(QString path1, QString path2, QTextEdit *textEdit = nullptr);//编码压缩
    void deCoding(QString path1, QString path2, QTextEdit *textEdit = nullptr);//解码



private:
    int getIndex(const char& c) const;
    void readChar();//hfmTree.txt读取字符及其频度
    void readChar(QString path);//依据待压缩的文本，自动读取，确定字符及其频度
    void iniCode(int parent, QString str);//递归，生成哈夫曼编码

//    char *ch;
    int n;//n个字符 根结点的索引号就是m-1
    node tree[200];//哈夫曼树 由于是英文文本，字符数（即结点数）必定少于100个 树的结点总个数必定少于200个
    MinHeap<node> *heap;//最小堆（辅助结构）
    QString *code;//哈夫曼编码

signals:

public slots:
};

#endif // HUFFMANTREE_H
