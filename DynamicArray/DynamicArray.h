#ifndef __DYNAMICARRAY_H_
#define __DYNAMICARRAY_H_

#define ElementType void*

struct DynamicArray
{
    ElementType *a;//堆上空间的首地址
    int size;//数组的大小
    int len;//元素长度
};
typedef struct DynamicArray DArray;

void InitDAaay(DArray *da);
//在尾部插入元素
void InsertTail(DArray *da, ElementType element);
//打印数组
void PrintArray(DArray *da);
//函数指针，指向的是一类函数(*f表示一个函数指针，省略函数名)
void PrintfDArray(DArray * da,void(*f)(ElementType));

#endif