#include <iostream>
#include <string>

using  std::cout;
using  std::cin;

void foo(int x) {
    std::cout << "Integer version: " << x << std::endl;
}

void foo(char* ptr) {
    std::cout << "Pointer version: " << ptr << std::endl;//优先使用nullptr 因为NULL的本质是(void*) 0
}      

void func(std::string  name ,std::string address ="汉族") //默认参数的一定是最右面的变量是默认值
{

    cout << name << ' ' << address;
}


void  func1(std::string  name ,std::string  = "zhangsan")  //占位参数只是为了解决函数重名的问题
{

    cout << name << ' ' ;
}
int main()
{
    std::string name = "张三";
    func(name);
    func1("lisi");
}