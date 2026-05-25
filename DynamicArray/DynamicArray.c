#include "DynamicArray.h"
#include <stdlib.h>
#include <stdio.h>

void InitDAaay(DArray *da)
{
    da->size = 10;
    da->a = (ElementType*)malloc(sizeof(ElementType) * da->size);
    if(da->a == NULL)
    {
        printf("malloc error!\n");
        return;
    }
    da->len = 0;
}

void InsertTail(DArray *da, ElementType element)
{
    //数组超过范围
    if(da->len == da->size)
    {
        da->size *= 2;
        ElementType *newP = (ElementType *)malloc(sizeof(ElementType) * da->size);
        if(newP == NULL)
        {
            printf("malloc error!\n");
            return;
        }
        for(int i = 0; i < da->len; i++)
        {
            newP[i] = da->a[i];
        }
        free(da->a);
        da->a = newP;
    }
    da->a[da->len] = element;
    da->len++;
}

// void PrintArray(DArray *da)
// {
//     printf("len = %d size = %d\n",da->len,da->size);
//     for(int i = 0; i < da->len; i++)
//     {
//         printf("%d ",da->a[i]);
//     }
//     printf("\n");
// }

void PrintfDArray(DArray *da, void (*f)(ElementType))
{
    printf("len = %d size = %d\n",da->len,da->size);
    for(int i = 0; i < da ->len; i++)
    {
        f(da->a[i]);
    }
    printf("\n");
}