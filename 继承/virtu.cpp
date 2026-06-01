// 虚继承 —— 解决菱形继承的"二义性"问题
//
// 问题场景（菱形继承）：
//      Person (基类，有 id 字段)
//     /     \
//  Teacher  Student
//     \     /
//     TA(助教)        ← TA 里会有两份 Person 的 id！
//
// 虚继承的作用：让共同基类 Person 在最终派生类中只存一份。

#include <iostream>
using namespace std;

// ============ 不加 virtual 的菱形继承（有问题）============
class Person {
protected:
    int id;
public:
    Person(int i) : id(i) {}
    int getId() const { return id; }
};

class Teacher : public Person {   // 普通继承，Teacher 里有一份 Person
public:
    Teacher(int i) : Person(i) {}
};

class Student : public Person {   // 普通继承，Student 里也有一份 Person
public:
    Student(int i) : Person(i) {}
};

class TA : public Teacher, public Student {
public:
    TA(int tId, int sId) : Teacher(tId), Student(sId) {}
    // TA 里有两份 Person（两份 id）！调用 getId() 会产生二义性
};

// ============ 加了 virtual 的菱形继承（问题解决）============
class Person2 {
protected:
    int id;
public:
    Person2(int i) : id(i) {}
    int getId() const { return id; }
};

class Teacher2 : virtual public Person2 {  // virtual 继承，共用同一份 Person2
public:
    Teacher2(int i) : Person2(i) {}
};

class Student2 : virtual public Person2 {  // virtual 继承，共用同一份 Person2
public:
    Student2(int i) : Person2(i) {}
};

class TA2 : public Teacher2, public Student2 {
public:
    TA2(int id) : Person2(id), Teacher2(id), Student2(id) {}
    // 关键：虚继承时，最终派生类 TA2 必须亲自调用虚基类 Person2 的构造函数！
    // Teacher2 和 Student2 对 Person2 的构造调用会被忽略。
};

int main() {
    // ====== 普通继承的问题 ======
    TA ta(100, 200);
    // ta.getId();   // 编译错误！不知道取 Teacher 的还是 Student 的 id
    cout << "普通继承 TA: id 有二义性，无法直接调用 getId()" << endl;
    cout << "  Teacher 的 id: " << ta.Teacher::getId() << endl;  // 100
    cout << "  Student 的 id: " << ta.Student::getId() << endl;  // 200

    // ====== 虚继承的解决 ======
    TA2 ta2(42);
    cout << "\n虚继承 TA2: 只有一份 id = " << ta2.getId() << endl;  // 42，无二义性

    // ====== 内存角度 ======
    cout << "\nsizeof(TA)  = " << sizeof(TA)  << " (两份 int id)" << endl;
    cout << "sizeof(TA2) = " << sizeof(TA2) << " (一份 int id + 虚基类表指针)" << endl;

    // ====== 总结 ======
    // ====== 虚函数表 vs 虚基类表：两者可同时存在 ======
    cout << "\n====== 虚函数表(vftable) vs 虚基类表(vbtable) ======" << endl;

    // 场景1：只有虚函数 → 有 vptr
    class HasVFunc {
        int x;
    public:
        virtual void f() {}   // 触发虚函数表 vftable
    };
    cout << "只有虚函数 sizeof=" << sizeof(HasVFunc) << endl;

    // 场景2：只有虚继承 → 有 vbptr
    class HasVInherit : virtual public Person2 {
        int x;
    public:
        HasVInherit(int i) : Person2(i) {}
    };
    cout << "只有虚继承 sizeof=" << sizeof(HasVInherit) << endl;

    // 场景3：同时有虚函数和虚继承 → 同时有 vptr 和 vbptr
    class HasBoth : virtual public Person2 {
        int x;
    public:
        HasBoth(int i) : Person2(i) {}
        virtual void f() {}
    };
    cout << "虚函数+虚继承 sizeof=" << sizeof(HasBoth)
         << " (同时有 vptr 和 vbptr)" << endl;

    // ====== 总结 ======
    cout << "\n--- 虚继承要点 ---" << endl;
    cout << "1. 解决菱形继承中共同基类成员的多份拷贝问题" << endl;
    cout << "2. 最终派生类必须亲自构造虚基类" << endl;
    cout << "3. 有轻微性能开销（虚基类表指针），只在必要时使用" << endl;
    cout << "4. 典型场景：iostream 中 istream/ostream 都对 ios 使用虚继承" << endl;
    cout << "\n--- 虚函数表 vs 虚基类表 ---" << endl;
    cout << "  虚函数表(vftable)：解决多态——调用【哪个】函数的地址   ← vptr" << endl;
    cout << "  虚基类表(vbtable)：解决菱形继承——基类【在哪儿】的偏移 ← vbptr" << endl;
    cout << "  两者互不干扰，一个类可以同时拥有两张表" << endl;

    return 0;
}
