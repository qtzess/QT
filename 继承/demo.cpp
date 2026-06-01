// C++ 继承在开发中的核心作用
// 1. 代码复用 —— 消除重复，公共逻辑抽取到基类
// 2. 多态     —— 统一接口操作不同子类对象
// 3. 扩展性   —— 新增子类不改现有代码（开闭原则）

#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// ============ 一、代码复用 ============
// 基类抽取公共字段和逻辑，子类只写差异化部分
class Animal {
protected:
    string name;
public:
    Animal(const string& n) : name(n) {}
    void sleep() { cout << name << " 在睡觉" << endl; } // 所有动物共用的方法
    virtual void speak() = 0; // 纯虚函数：每个子类必须实现
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    Dog(const string& n) : Animal(n) {}
    void speak() override { cout << name << " 汪汪叫" << endl; }
};

class Cat : public Animal {
public:
    Cat(const string& n) : Animal(n) {}
    void speak() override { cout << name << " 喵喵叫" << endl; }
};

// ============ 二、多态 ============
// 用基类指针/引用统一操作不同子类
void chorus(vector<unique_ptr<Animal>>& animals) {
    for (auto& a : animals) {
        a->speak(); // 同一行代码，根据实际类型调用不同实现
    }
}

// ============ 三、扩展性演示 ============
// 新增一个子类，chorus() 无需任何修改
class Duck : public Animal {
public:
    Duck(const string& n) : Animal(n) {}
    void speak() override { cout << name << " 嘎嘎叫" << endl; }
};

int main() {
    // 用基类指针管理不同子类 —— 这才是继承的真正威力
    vector<unique_ptr<Animal>> animals;
    animals.push_back(make_unique<Dog>("旺财"));
    animals.push_back(make_unique<Cat>("咪咪"));
    animals.push_back(make_unique<Duck>("唐老鸭")); // 后加的子类无缝接入

    // 复用基类的 sleep()
    for (auto& a : animals) a->sleep();

    // 多态：同一个 speak() 调用，不同的行为
    cout << "\n--- 大合唱 ---" << endl;
    chorus(animals);

    return 0;
}
