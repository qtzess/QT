#include <iostream>

/*
模板：类型参数化
优先调用普通函数
*/
template <typename T>
void func(T a,T b)
{


    std::cout << a << b <<std::endl;

}


template <typename T,typename T1>
class student
{
    public:
    student(T n, T1 a):name(n),age(a)
    {

    }
    student(student& t)
    {


    }
    void show()
    {
        std::cout << name << " " << age <<std::endl;
    }
    
    template<typename U>
    friend std::ostream& operator<<( std::ostream& os,const U &da);
    private:
    T name;
    T1  age;
};



 template<typename U>
    std::ostream& operator<<( std::ostream& os,const U &da)
    {
        

    }
   
int main()
{

    func(10,20);
    func(10.5,20.5);
    func<int>(1,2); //模板的现实调用


    student<std::string,int> p ("zhang",18);
  
    p.show();
    return 0;
}