#include "huffmantree.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDataStream>
#include <QChar>
#include <QQueue>

#include <fstream>//c++自带的文件操作
#include <iostream>
using namespace std;


//构造函数
HuffmanTree::HuffmanTree(QObject *parent, QString path)
{
    ////////读取字符 初始化ch weight index parent child[2]
    if(path == "")
        readChar();
    else
        readChar(path);
    qDebug() << n << 2 * n - 1;
    for(int i = 0; i < 2 * n - 1; i++)
    {
        tree[i].index = i;
        tree[i].parent = tree[i].child[0] = tree[i].child[1] = -1;
    }

    /////建堆
    heap = new MinHeap<node>(n);//建立最小堆
    for(int i = 0; i < n; i++)
        heap->put(tree[i]);//压入堆中

    ////////建树
    for(int i = n; i < 2 * n - 1; i++)
    {
        node n1 = heap->get();//取最小权值的结点
        node n2 = heap->get();//取次小权值的结点
//        qDebug() << ch[n1.index] << ch[n2.index];
        tree[n1.index].parent = tree[n2.index].parent = i;//将其父结点设为最前的非叶结点
        tree[i].weight = n1.weight + n2.weight;//将此非叶结点的权值设为两最小结点之和
        tree[i].child[0] = n1.index;
        tree[i].child[1] = n2.index;
//        tree[i].ch = 'a' + i;
//        qDebug() << tree[i].weight;
        heap->put(tree[i]);//将新得的结点权值压入最小堆中
    }

    ///////组织哈夫曼编码
    code = new QString[n];//新建编码字符串数组
    iniCode(2 * n - 2, 0);//组织编码，本质上是一个二叉树的前序遍历
}

//从hfmTree.txt读取字符及其频度
void HuffmanTree::readChar()
{
    /////以只读模式打开文件
    QFile file("hfmTree.txt");
    file.open(QIODevice::ReadOnly);

    /////获取字符数量
    QByteArray array = file.readLine();
    QTextStream ts(array);
    ts >> n;
    qDebug() << n;

    //////继续读入各字符及其权值
    int i = 0;
    while(!file.atEnd())
    {
        array = file.readLine();//逐行读取
//        qDebug() << array;
//        ch[i] = array.at(0);//取读到的第1个字符
        tree[i].ch = array.at(0);//取读到的第1个字符
        QTextStream ts2(array.mid(2));//从第3个字符开始读取数字
        ts2 >> tree[i].weight;
//        qDebug() << ch[i] << tree[i].weight;
        i++;
    }
    file.close();//关闭文件
}

//依据待压缩的文本，自动读取，确定字符及其频度
void HuffmanTree::readChar(QString path)
{
    /////打开文件
    ifstream toBeTran(path.toUtf8().data(),ios::in | ios::binary);
    if(!toBeTran)
    {
        qDebug() << "toBeTran文件打开失败!";
        return;
    }

    ////依次读取单个字符并逐一记录
    n = 0;
    char ch;
    while(toBeTran.read((char*)&ch,sizeof(ch)))
    {
        int index = getIndex(ch);
        /*if(index == -1)//如果该字符未曾出现过 ->不可能！！！
        {
            tree[n].ch = ch;
            tree[n].weight = 1;
            n++;
        }*/
        /*else//如果该字符已经存在于哈夫曼树中
        {*/
            tree[index].weight++;
            qDebug() << tree[index].ch << tree[index].weight;
        /*}*/
    }
    toBeTran.close();

    //////存入hfmTree.txt
    ofstream out("hfmTree.txt",ios::out | ios::ate);
    if(!out)
    {
        qDebug() << "hfmTree文件打开失败!";
        return;
    }
    out << n << endl;
    for(int i = 0; i < n; i++)
    {
        out << tree[i].ch << " " << tree[i].weight << endl;
    }
    out.close();
}

//组织哈夫曼编码的子过程，递归
void HuffmanTree::iniCode(int parent, QString str)
{
//    qDebug() << parent << str;
    if(parent < 0 || parent >= 2*n-1)//如果父结点为空或数值异常，则结束递归
        return ;

    if(tree[parent].child[0] == -1 && tree[parent].child[1] == -1)//如果到了叶子结点
    {
        code[parent] = str.toLatin1();
//        qDebug() << "√";
        return ;
    }
    iniCode(tree[parent].child[0], str+"0");
    iniCode(tree[parent].child[1], str+"1");
}

//打印 层次遍历
void HuffmanTree::print(QLabel* label)
{
    int i, m = 2 * n - 1;
    qDebug() << "哈夫曼树";
    for(i = 0; i < m; i++)
        qDebug() << "序号：" << i << "左孩子" << tree[i].child[0] << "右孩子" << tree[i].child[1]
                 << "权值" << tree[i].weight << "双亲结点" << tree[i].parent;
    qDebug() << "哈夫曼编码";
    for(i = 0; i < n; i++)
        qDebug() << tree[i].ch << code[i];

    //层次遍历 获取结点序列
    QQueue<int> q1,q2;
    q1.enqueue(m - 1);
    q2.enqueue(m - 1);
    while(!q1.empty())
    {
        int i = q1.dequeue();
        if(i != -1)
        {
            q1.enqueue(tree[i].child[0]);
            q1.enqueue(tree[i].child[1]);
            q2.enqueue(tree[i].child[0]);
            q2.enqueue(tree[i].child[1]);
        }
        else
        {
            q2.enqueue(-1);
            q2.enqueue(-1);
        }
    }
    //打印二叉树
    QString str;
    int layer = 1, number = 0, current = 0;//层次 完全二叉树中的第number个节点 当前结点
    while(!q2.empty())
    {
        number = pow(2, layer) - 1;//完全二叉树每层的最后一个节点的编号为2的layer次方-1
//        qDebug() << number;
        QString str2;
        for(; current < number; current++)//逐层输出
        {
            if(q2.empty())
                break;
            int i = q2.dequeue();
            if(i < 0)
                str += " ";
            else if(i < n)
                str += tree[i].ch;
//                str1 += ch[i];
            else
                str += "●";

        }
        str += '\n';
        layer++;
    }

    //展示
    if(label != 0)
        label->setText(str);
    ofstream treePrinting("TreePrinting.txt", ios::out);//以只写模式打开
    if(!treePrinting)
    {
        qDebug() << "生成treePrinting.txt,文件打开失败!";
        return;
    }
    else
    {
        char *c = str.toUtf8().data();
        treePrinting << c;
    }

}

//编码压缩
void HuffmanTree::enCoding(QString path1, QString path2, QTextEdit *textEdit)
{   
    //////编码
    ifstream toBeTran(path1.toUtf8().data(), ios::in);
    if(!toBeTran)
    {
        qDebug() << "toBeTran文件打开失败!";
        return;
    }
    char ch;
    QString str;
    while(toBeTran.read((char*)&ch,sizeof(ch)))
    {
        int index = getIndex(ch);//取该字符的索引号
        if( index >= 0 )//若该字符已存在于哈夫曼树中（已被编码）
            str += code[index];//则转换为哈夫曼编码
    }
    toBeTran.close();
    qDebug() << "编码完成";

    /////生成CodePrin.txt
    ofstream codePrin("CodePrin.txt", ios::out);//以只写模式打开
    if(!codePrin)
    {
        qDebug() << "生成CodeFile.txt,文件打开失败!";
        return;
    }
    else
    {
        char *c = str.toUtf8().data();
        codePrin << c;
        qDebug() << "写入CodeFile.txt成功!";
    }

    /////写入控件
    if(textEdit)//当控件存在时
    {
        textEdit->setText(str);
        qDebug() << "压缩结果 写入文本框√";
    }

    ///////生成CodeFile.dat
    int length = str.size();
    qDebug() << length;

    ofstream codeFile(path2.toUtf8().data(), ios::out | ios::binary);//以只写、二进制模式打开
    if(!codeFile)
    {
        qDebug() << "生成CodeFile.txt,文件打开失败!";
        return;
    }
    else
    {
        for(int i = 0; i < length;)
        {
            char x = 0, ch;
            for (int j = 0; j < 8; j++)
            {
                x = x << 1;     // bit op
                if(i < length)
                {
                    ch = str.at(i).unicode();
                    i++;
                }
                x = ch & 0x01 | x;
            }
            codeFile.write((char*)&x, sizeof(x));
        }
        codeFile.close();
        qDebug() << "压缩结果写入CodeFile.dat√";
    }
}

//解码
void HuffmanTree::deCoding(QString path1, QString path2, QTextEdit *textEdit)
{
    ofstream textFile(path2.toUtf8().data(),ios::out);
    if(!textFile)
    {
        qDebug() << "textFile文件打开失败!";
        return;
    }

    ifstream codeFile(path1.toUtf8().data(),ios::in | ios::binary);
    if(!codeFile)
    {
        qDebug() << "codeFile文件打开失败!";
        return;
    }
    int code, root = 2 * n - 2, index = root;//从根节点开始检索
    char x;
    while(codeFile.read((char*)&x,sizeof(x)))
    {
        for (int i = 0; i < 8; i++)
        {
            code = x & 0x80;
            code = code >> 7;
            if (code == 0)
                index = tree[index].child[0];
            else
                index = tree[index].child[1];
            if(tree[index].child[0] == -1 && tree[index].child[1] == -1)//检索到叶子结点
            {
                textFile << tree[index].ch;
                index = root;
            }
            x=x<<1;
        }
    }
    textFile.close();
    codeFile.close();

    if(textEdit)//当控件存在时
    {
        QFile f(path2);
        if(f.open(QIODevice::ReadOnly))
        {
            QByteArray array = f.readAll();
            textEdit->setText(QString(array));
            f.close();
            qDebug() << "解压结果 写入文本框√";
        }
        else
        {
            qDebug() << "解压结果无法写入文本框";
        }
    }
}

//获取字符c在哈夫曼树数组中对应的索引号
int HuffmanTree::getIndex(const char& c) const
{
    for(int i = 0; i < n; i++)
    {
        if(tree[i].ch == c)
            return i;
    }
    return -1;
}
