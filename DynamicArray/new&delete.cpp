
int main()

{


     int  *ptr = new int;  //申请一个整型数据
     *ptr = 10;  
     delete ptr;

    int *ptr1 = new int[5];
    for(int i = 0 ; i < 5;i++)
    {
        ptr[i] = i+3;
    }
    delete[] ptr1;

    int **ptr2 = new int*[5];
    for(int i = 0 ;i  < 5;i++)
    {
        ptr2[i] = new int[3];
    }
    
    for(int i = 0 ;i  < 5;i++)
    {
        delete [] ptr2[i];

    }
    delete[]ptr2;
}