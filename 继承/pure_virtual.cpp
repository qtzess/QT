// 纯虚函数 = 接口契约 + 禁止实例化基类
//
// 语法：virtual 返回类型 函数名() = 0;
//
// 两个核心作用：
// 1. 强制子类实现 —— 基类说"我不管你怎么做，但你必须提供这个功能"
// 2. 基类不能实例化 —— "Shape 只是个概念，你不能 new 一个形状"

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
constexpr double PI = 3.14159265358979323846;
using namespace std;

// ============ 抽象基类：只定义接口 ============
class Shape {
public:
    virtual double area() const = 0;      // 纯虚函数：面积
    virtual double perimeter() const = 0; // 纯虚函数：周长
    virtual void print() const = 0;       // 纯虚函数：打印信息

    // 非纯虚函数也是允许的（提供默认实现）
    void describe() const {
        cout << "这是一个形状，面积=" << area() << "，周长=" << perimeter() << endl;
    }

    virtual ~Shape() = default;
    
};

// Shape s;  // 编译错误！不能实例化含有纯虚函数的类

// ============ 子类：必须实现所有纯虚函数 ============
class Circle : public Shape {
    double r;
public:
    Circle(double radius) : r(radius) {}
    double area() const override      { return PI * r * r; }
    double perimeter() const override { return 2 * PI * r; }
    void print() const override       { cout << "圆形(半径=" << r << ")"; }
};

class Rectangle : public Shape {
    double w, h;
public:
    Rectangle(double width, double height) : w(width), h(height) {}
    double area() const override      { return w * h; }
    double perimeter() const override { return 2 * (w + h); }
    void print() const override       { cout << "矩形(宽=" << w << ", 高=" << h << ")"; }
};

class Triangle : public Shape {
    double a, b, c;
public:
    Triangle(double x, double y, double z) : a(x), b(y), c(z) {}
    double area() const override {
        double s = (a + b + c) / 2;
        return sqrt(s * (s - a) * (s - b) * (s - c)); // 海伦公式
    }
    double perimeter() const override { return a + b + c; }
    void print() const override       { cout << "三角形(三边=" << a << "," << b << "," << c << ")"; }
};

// ============ 上层代码只依赖抽象接口 ============
double totalArea(const vector<unique_ptr<Shape>>& shapes) {
    double sum = 0;
    for (auto& s : shapes)
        sum += s->area();   // 多态调用，无需知道具体类型
    return sum;
}

int main() {
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(3.0));
    shapes.push_back(make_unique<Rectangle>(4.0, 5.0));
    shapes.push_back(make_unique<Triangle>(3.0, 4.0, 5.0));

    // 统一接口操作不同形状
    for (auto& s : shapes) {
        s->print();
        cout << " → ";
        s->describe();
    }

    cout << "\n所有形状总面积: " << totalArea(shapes) << endl;

    // ====== 场景：新增形状不改现有代码 ======
    class Square : public Shape {
        double side;
    public:
        Square(double s) : side(s) {}
        double area() const override      { return side * side; }
        double perimeter() const override { return 4 * side; }
        void print() const override       { cout << "正方形(边长=" << side << ")"; }
    };

    shapes.push_back(make_unique<Square>(6.0));
    cout << "\n加入正方形后总面积: " << totalArea(shapes) << endl;

    // ====== 总结 ======
    cout << "\n--- 纯虚函数的作用 ---" << endl;
    cout << "1. 定义接口契约 —— 子类必须实现，编译器强制检查" << endl;
    cout << "2. 禁止实例化基类 —— 抽象概念不能 new（Shape 是抽象概念）" << endl;
    cout << "3. 面向接口编程 —— 上层代码依赖 Shape*，不依赖具体形状" << endl;
    cout << "4. 开闭原则 —— 新增形状类不改 totalArea() 一行代码" << endl;

    return 0;
}
