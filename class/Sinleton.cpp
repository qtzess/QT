#include <iostream>

/*1、取消复制运算符和拷贝构造函数
  2、私有化构造函数，防止直接私有化
  3、静态函数申请一个静态变量，返回对象的地址


*/
class Sinleton
{
private:
    /* data */
    Sinleton(/* args */) {

    };

    Sinleton(const Sinleton &T) = delete;
    Sinleton &operator=(const Sinleton &T) = delete;

public:
    static Sinleton *Getintance()
    {
        static Sinleton singletonN;
        return &singletonN;
    }
};

int main()
{

    Sinleton *one = Sinleton::Getintance();
    Sinleton *two = Sinleton::Getintance();
    Sinleton *three = Sinleton::Getintance();

    std::cout << one << " "<< two <<std::endl; 
}