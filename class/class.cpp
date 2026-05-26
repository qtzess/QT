#include <iostream>


class Student
{
    public:
    void show();
    void setName(const Student& s);

    Student()
    {
        a= new int;
        
    }
      Student(Student & T)
    {
        a= new int;
        *a = *T.a;  //深度拷贝：申请自己的内存，只是复制形参的值
        
    }
    
    ~Student()
    {
        delete a;    //RAII思想策略  构造函数中申请，析构函数中释放
        //浅拷贝的问题  构造函数中申请，析构函数中释放，如果拷贝构造造成两个对象指向一个地址，会出现重复析构的问题
    }

    private:
    int *a;
    std::string name;
};


int main()
{
    Student s;
    s.show();

}


void Student::show()
{


}

void Student::setName(const Student& s)//这里的const作用 1、强制型参数的值不可以该改变 2、可以绑定一些临时变量 比如"张三"
{


}
