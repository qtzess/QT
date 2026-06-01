#include <iostream>
#include <memory>

/*
 * ============================================================
 * weak_ptr — 解决 shared_ptr 循环引用
 * ============================================================
 *
 * 问题: 两个 shared_ptr 互相指着对方 → 计数器永远不为0 → 内存泄漏
 * 方案: 一方改用 weak_ptr (不增加计数) → 打破了循环
 */

class B;  // 前置声明

class A {
public:
    A()  { std::cout << "[构造] A\n"; }
    ~A() { std::cout << "[析构] A\n"; }

    std::shared_ptr<B> ptrB;  // A 持有 B 的 shared_ptr
};

class B {
public:
    B()  { std::cout << "[构造] B\n"; }
    ~B() { std::cout << "[析构] B\n"; }

    std::weak_ptr<A> ptrA;  // 👈 B 用 weak_ptr 指回 A, 不增加计数!
};

int main() {

    // ============================================================
    std::cout << "========== 循环引用与 weak_ptr 的解决 ==========\n\n";
    {
        auto a = std::make_shared<A>();
        auto b = std::make_shared<B>();

        std::cout << "引用前:\n";
        std::cout << "  a 计数 = " << a.use_count() << "\n";
        std::cout << "  b 计数 = " << b.use_count() << "\n\n";

        a->ptrB = b;   // A 指向 B: b 计数 = 2 (外部 + a 内部)
        b->ptrA = a;   // B 指向 A: weak_ptr 不增加计数 → a 仍为 1!

        std::cout << "引用后:\n";
        std::cout << "  a 计数 = " << a.use_count() << " ← weak_ptr 不增加!\n";
        std::cout << "  b 计数 = " << b.use_count() << " ← shared_ptr 增加了\n\n";

    } // a 析构: 计数 1→0 → delete A
      //    A 析构 → ptrB 也析构 → b 计数 2→1
      // b 析构: 计数 1→0 → delete B
      // ✅ 两个都正确释放!

    std::cout << "✅ A 和 B 都正确析构了\n\n";

    // ============================================================
    // 对比: 如果 B 也用的 shared_ptr<A>
    // ============================================================
    std::cout << "========== 如果双方都是 shared_ptr 会怎样? ==========\n";
    std::cout << "  引用关系:\n";
    std::cout << "    a.ptrB → b     (b 计数+1)\n";
    std::cout << "    b.ptrA → a     (a 计数+1)\n";
    std::cout << "  离开作用域:\n";
    std::cout << "    a 析构 → 计数 2→1, 不释放!\n";
    std::cout << "    b 析构 → 计数 2→1, 也不释放!\n";
    std::cout << "  结果: 互相抱着一起泄漏 💀\n\n";

    // ============================================================
    std::cout << "========== weak_ptr.lock() — 安全检查 ==========\n";
    std::cout << "  weak_ptr 不能直接使用, 要先 .lock():\n\n";

    std::weak_ptr<int> wp;
    {
        auto sp = std::make_shared<int>(42);
        wp = sp;  // weak_ptr 观察 sp 的资源

        if (auto p = wp.lock()) {  // lock() 返回 shared_ptr
            std::cout << "  资源存活, 值 = " << *p << "\n";
        }
    } // sp 析构, 资源释放

    if (auto p = wp.lock()) {
        std::cout << "  值 = " << *p << "\n";
    } else {
        std::cout << "  资源已释放, lock() 返回空 — 安全!\n";
    }

    std::cout << "\n总结: 循环引用的一方用 weak_ptr → 打破计数循环 → 正确释放\n";
    return 0;
}
