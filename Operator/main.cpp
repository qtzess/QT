
#include <iostream>

class Complex
{

    private:
    int real;
    int vir;

    public:
    
    friend std::ostream& operator<<(std::ostream &os,Complex& t );
    Complex operator+(const Complex& T)
    {
        this->real += T.real;
        this->vir  += T.vir;
        return *this;

    }

};


/*输入输出运算符重载的三要素
1、全局：为了方便习惯
2、在类里设置友元函数，方便查看类中的私有变量
3、返回引用，为了多级使用 */
std::ostream& operator<<(std::ostream &os,Complex& t ) //左值一定会发生拷贝构造

{

    os << t.real ;
    return os;

}
int main()
{

    Complex t,t1;
    Complex t3 = t + t1;
}