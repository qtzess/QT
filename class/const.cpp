#include <iostream>
#include <string>

class Person {
public:
    std::string name;

    Person(std::string n) : name(n) {}

    // 1. const 返回值 — 返回的数据不可修改
    const std::string& getName() const {
        return name;
    }

    // 2. const 形参 — 承诺不修改传入的对象
    void show(const Person& p) const {
        std::cout << p.name << std::endl;
    }

    // 3. const 常成员函数 — 承诺不修改本对象的成员
    void print() const {
        // name = "xxx";  // 编译错误！const 函数不能改成员
        std::cout << "print: " << name << std::endl;
    }
};

int main() {
    Person p("张三");
    const Person& ref = p;

    // 返回值是 const 引用，不可通过它修改
    const std::string& n = p.getName();
    // n = "李四";  // 错误！const 返回值不可赋值

    // 常成员函数 — const 对象只能调用 const 成员函数
    ref.print();  // OK，print 是 const 函数

    // 传 const 引用进去
    p.show(p);

    return 0;
}
