#include <iostream>

// 主模板（Primary Template）
template <typename T, typename U>
class Pair {
public:
    void info() const {
        std::cout << "主模板: T, U" << std::endl;
    }
};

// 偏特化（Partial Specialization）：第二个参数固定为 int
// 只要 U = int，就匹配这个版本
template <typename T>
class Pair<T, int> {
public:
    void info() const {
        std::cout << "偏特化: T, int" << std::endl;
    }
};

// 偏特化：两个参数类型相同
template <typename T>
class Pair<T, T> {
public:
    void info() const {
        std::cout << "偏特化: T, T (同类型)" << std::endl;
    }
};

// 偏特化：两个参数都是指针
template <typename T, typename U>
class Pair<T*, U*> {
public:
    void info() const {
        std::cout << "偏特化: T*, U* (两个指针)" << std::endl;
    }
};

// 全特化（Full Specialization）：两个参数都确定了
// 只有 Pair<int, double> 匹配
template <>
class Pair<int, double> {
public:
    void info() const {
        std::cout << "全特化: int, double" << std::endl;
    }
};

int main() {
    Pair<char, float>  p1;   // 主模板
    Pair<char, int>    p2;   // 偏特化 T,int
    Pair<double, double> p3;  // 偏特化 T,T
    Pair<int*, char*>  p4;   // 偏特化 T*,U*
    Pair<int, double>  p5;   // 全特化
 //匹配优先级：全特化 > 偏特化 > 主模板。多个偏特化都匹配时，选"更特化"的那个
    p1.info();
    p2.info();
    p3.info();
    p4.info();
    p5.info();

    return 0;
}
