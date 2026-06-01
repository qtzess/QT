#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

/*
 * ============================================================
 * C++ 异常处理 — try / catch / throw
 * ============================================================
 *
 * 核心概念:
 *   throw  — 抛出异常, 栈会逐层展开 (stack unwinding)
 *   try    — 包裹可能出异常的代码
 *   catch  — 捕获异常, 类型必须严格匹配 (或 catch(...) 兜底)
 *
 * 异常 + RAII = 天然异常安全:
 *   栈展开时, 所有栈上的对象都会自动析构,
 *   智能指针确保堆上的资源也会被回收, 不会泄漏!
 */

// ============================================================
// 1. 基本: throw 与 catch 的类型匹配
// ============================================================
int divide(int a, int b) {
    if (b == 0)
        throw "除数为零!";   // 抛出 const char*
    return a / b;
}

// ============================================================
// 2. 自定义异常类 (推荐做法)
// ============================================================
class MyException : public std::runtime_error {
public:
    MyException(const std::string& msg)
        : std::runtime_error(msg) {}
};

// ============================================================
// 3. 带 RAII 资源的类 — 演示栈展开时自动析构
// ============================================================
class Resource {
public:
    Resource(int id) : id_(id) {
        std::cout << "  [构造] Resource(" << id_ << ")\n";
    }
    ~Resource() {
        std::cout << "  [析构] Resource(" << id_ << ") — RAII 自动回收\n";
    }
private:
    int id_;
};

// 演示: 函数中间抛异常, 局部对象仍然正确析构
void funcThatThrows() {
    Resource r1(1);                              // 栈上对象
    auto    r2 = std::make_unique<Resource>(2);  // 堆上对象 (智能指针)

    std::cout << "  funcThatThrows: 即将抛出异常...\n";
    throw std::runtime_error("出错了!");

    // 下面的代码永远不会执行
    std::cout << "  这行不会打印\n";
}
// r1 和 r2 都会自动析构 ← 这就是 RAII 的异常安全!

// ============================================================
// 4. noexcept: 承诺不抛异常, 但如果抛了 → std::terminate
// ============================================================
void safeFunc() noexcept {
    std::cout << "  safeFunc: 这段是 noexcept, 承诺不抛异常\n";
    // throw 0;  // ❌ 取消注释会直接调用 std::terminate, 程序崩溃!
}

// ============================================================
int main() {

    // ======================================================
    std::cout << "========== 1. 基本 try-catch ==========\n\n";
    {
        try {
            std::cout << "  执行 10/0...\n";
            std::cout << divide(10, 0) << std::endl;
        }
        catch (int e) {
            // 只捕获 int 类型的异常
            std::cout << "  ❌ 不会走到这里, 因为抛的是 const char*\n";
        }
        catch (const char* msg) {
            // 类型必须精确匹配! const char* 不能由 char* 的 catch 捕获
            std::cout << "  ✅ 捕获到: " << msg << "\n";
        }
        catch (...) {
            // 兜底: 捕获所有类型
            std::cout << "  ❌ 不会走到这里, 上面已经匹配了\n";
        }
    }
    std::cout << "\n";

    // ======================================================
    std::cout << "========== 2. catch(...) 兜底 ==========\n\n";
    {
        try {
            throw 'A';  // 抛 char 类型 — 没有精确匹配的 catch
        }
        catch (int e) {
            std::cout << "  ❌ 不会匹配 char\n";
        }
        catch (...) {
            std::cout << "  ✅ catch(...) 捕获了 char 异常 (没有其他匹配)\n";
        }
    }
    std::cout << "\n";

    // ======================================================
    std::cout << "========== 3. 异常 + RAII = 自动资源回收 ==========\n\n";
    {
        try {
            funcThatThrows();
        }
        catch (const std::exception& e) {
            std::cout << "  catch: " << e.what() << "\n";
        }
        std::cout << "  → 即使抛异常, Resource(1) 和 Resource(2) 都析构了!\n";
        std::cout << "  → 这就是「异常安全」: 不会泄漏资源\n";
    }
    std::cout << "\n";

    // ======================================================
    std::cout << "========== 4. 自定义异常类 ==========\n\n";
    {
        try {
            throw MyException("文件打开失败");
        }
        catch (const MyException& e) {
            std::cout << "  ✅ 捕获自定义异常: " << e.what() << "\n";
        }
        catch (const std::exception& e) {
            // MyException 继承自 std::runtime_error → std::exception
            // 如果上面不匹配, 这里也能捕获 (多态)
            std::cout << "  ⚠️  不会到这里, MyException 已被上面捕获\n";
        }
    }
    std::cout << "\n";

    // ======================================================
    std::cout << "========== 5. noexcept ==========\n\n";
    {
        safeFunc();
        std::cout << "  noexcept 函数: 编译器知道不会抛异常, 可以优化\n";
        std::cout << "  如果 noexcept 函数内 throw → std::terminate → 程序终止\n";
    }
    std::cout << "\n";

    // ======================================================
    std::cout << "========== 总结 ==========\n\n";
    std::cout <<
        "  throw:              抛异常, 栈展开\n"
        "  try { ... }         尝试执行\n"
        "  catch(Type e) { }   精确匹配异常类型\n"
        "  catch(...) { }      兜底, 捕获一切\n"
        "\n"
        "  关键规则:\n"
        "  1. 类型必须严格匹配 (隐式转换不算)\n"
        "     throw 'A' → catch(char), 不是 catch(int)\n"
        "  2. 继承可以匹配: 子类能被父类 catch 捕获\n"
        "  3. 建议抛类对象, 不要抛基本类型\n"
        "  4. RAII + 异常 = 异常安全: 栈展开自动析构\n"
        "  5. 析构函数里不要抛异常 (会导致 std::terminate)\n"
        "  6. noexcept 标记不抛异常的函数\n";

    return 0;
}
