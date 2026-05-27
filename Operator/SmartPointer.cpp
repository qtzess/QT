#include <iostream>

class T
{
    private:
    int a;

    public:
    T():a(10)
    {

    }


};
class StmartPointer
{
    private:
    T * t;

    public:
    ~StmartPointer() //只会释放指针变量，不会释放掉指针变量值 
    {
        delete t;
    }
    StmartPointer(T* t)
    {
        this->t = t; 
    }



};

int main()
{

    StmartPointer(new T);
}