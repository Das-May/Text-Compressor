#ifndef MINHEAP_H
#define MINHEAP_H

#include <QDebug>
//#include <QObject>
//我惊了，模板类不能继承QObject,所以报错[debug/moc_minheap.cpp] Error 1
template <class T>
class MinHeap
{
    //Q_OBJECT//如果自定义类使用了信号/槽机制，就需要在定义类时加上Q_OBJECT宏，否则singnals、slots定义编译器无法识别
public:
    //构造函数
    MinHeap(int n = 1):maxSize(n)
    {
        heap = new T[n];
        if(!heap)
        {
            abort();
//            emit error();
        }
        currentSize = 0;
    }

    //插入元素
    void put(T data)
    {
        heap[currentSize] = data;
        currentSize++;
        //上滑调整
        int i = currentSize - 1;
        int j = i / 2;
        T temp = heap[i];
        while(i > 0)
        {
            if(heap[j] < temp)
                break;
            else
            {
                heap[i] = heap[j];//我是不是上下滑分不清啊？？？？
                i = j;
                j = j / 2;
            }
        }
        heap[i] = temp;
//        qDebug() << "插入元素成功！";
    }

    //弹出并获取堆顶元素
    T get()
    {
        T top = heap[0];
        currentSize--;
        heap[0] = heap[currentSize];
        //下滑调整
        int i = 0;
        int j = i * 2 + 1;//左孩子
        T temp = heap[i];
        while(j < currentSize)
        {
            if(heap[j + 1] < heap[j])//如果右孩子更小，则转为右孩子
                j++;
            if(heap[i] < heap[j])
                break;
            else
            {
                heap[i] = heap[j];
                i = j;
                j = j * 2 + 1;
            }
        }
        heap[i] = temp;
//        qDebug() << "弹出堆顶元素！";
        return top;
    }

    ~MinHeap()
    {
        delete heap;
    }

private:
    T *heap;
    int maxSize, currentSize;

};

#endif // MINHEAP_H
