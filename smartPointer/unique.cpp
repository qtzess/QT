#include <iostream>
#include <string>

// ============================================================
// 独占智能指针的实现原理
// ============================================================

/*
 * unique_ptr 的核心机制:
 *
 *   1. RAII (Resource Acquisition Is Initialization)
 *      构造时获取资源 (new), 析构时释放资源 (delete)
 *
 *   2. 禁止拷贝 = delete
 *      拷贝构造和拷贝赋值被删除, 保证"独占"语义
 *
 *   3. 支持移动
 *      通过移动构造和移动赋值转移所有权
 *      转移后, 源指针置为 nullptr
 */

// ============================================================
// 一个最简独占智能指针的实现
// ============================================================
template <typename T>
class my_unique_ptr {
public:
    // ---- 1. 构造: 获取资源 (RAII 的 R) ----
    explicit my_unique_ptr(T* ptr = nullptr) : ptr_(ptr) {}

    // ---- 2. 析构: 释放资源 (RAII 的 I) ----
    //      对象离开作用域时自动调用, 保证资源不泄漏
    ~my_unique_ptr() {
        if (ptr_) {
            delete ptr_;
            ptr_ = nullptr;
        }
    }

    // ---- 3. 禁止拷贝: 独占语义的核心 ----
    //      如果允许拷贝, 两个指针指向同一块内存,
    //      析构时 delete 两次 → double free → 未定义行为!
    my_unique_ptr(const my_unique_ptr&) = delete;
    my_unique_ptr& operator=(const my_unique_ptr&) = delete;

    // ---- 4. 允许移动: 所有权转移 ----
    //      资源从 src 转移到 this, src 变成空
    my_unique_ptr(my_unique_ptr&& src) noexcept : ptr_(src.ptr_) {
        src.ptr_ = nullptr;  // 关键: 源指针置空, 防止 double free
    }

    my_unique_ptr& operator=(my_unique_ptr&& src) noexcept {
        if (this != &src) {
            if (ptr_) delete ptr_;   // 先释放自己原有的资源
            ptr_ = src.ptr_;         // 接管 src 的资源
            src.ptr_ = nullptr;      // src 置空
        }
        return *this;
    }

    // ---- 5. 指针操作 ----
    T* get()             const { return ptr_; }
    T* operator->()      const { return ptr_; }
    T& operator*()       const { return *ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }

private:
    T* ptr_;
};

// ============================================================
// 用 student 类做 demo
// ============================================================
class Student {
public:
    Student(std::string name, int age)
        : name_(std::move(name)), age_(age) {
        std::cout << "[构造] Student(" << name_ << ", " << age_ << ") 创建\n";
    }

    ~Student() {
        std::cout << "[析构] Student(" << name_ << ", " << age_ << ") 销毁\n";
    }

    void introduce() const {
        std::cout << "  我叫 " << name_ << ", 今年 " << age_ << " 岁\n";
    }

private:
    std::string name_;
    int age_;
};

// ============================================================
int main() {
    std::cout << "========== 1. 构造 — 获取资源 ==========\n";
    {
        // 堆上创建对象, my_unique_ptr 接管所有权
        my_unique_ptr<Student> s1(new Student("张三", 20));

        // 用 -> 访问成员
        s1->introduce();
    } // <-- s1 离开作用域, 析构函数自动 delete — RAII!
    std::cout << "  (s1 已被自动回收)\n\n";

    // ============================================================
    std::cout << "========== 2. 拷贝 — 编译期禁止! ==========\n";
    {
        my_unique_ptr<Student> s1(new Student("李四", 21));

        // ❌ 下面两行取消注释会编译失败:
        // my_unique_ptr<Student> s2 = s1;   // 拷贝构造 = delete
        // my_unique_ptr<Student> s3;
        // s3 = s1;                          // 拷贝赋值 = delete

        std::cout << "  s1 独占 Student(李四, 21)\n";
        std::cout << "  拷贝操作在编译期就被阻止了\n\n";

        /*
         * 为什么必须禁止拷贝?
         *
         *   如果允许拷贝, s1 和 s2 内部的 ptr_ 会指向同一块内存:
         *
         *     s1.ptr_ ──→ [Student 对象]
         *     s2.ptr_ ──→ [同一块内存]
         *
         *   s1 析构 → delete → Student 对象被释放
         *   s2 析构 → delete → 同一块内存再被 delete → double free!
         *
         *   这就是 undefined behavior, 程序可能崩溃。
         *   通过 = delete, 这个 bug 被消灭在编译期。
         */
    }
    std::cout << "\n";

    // ============================================================
    std::cout << "========== 3. 移动 — 转移所有权 ==========\n";
    {
        my_unique_ptr<Student> s1(new Student("王五", 22));
        std::cout << "  移动前: s1 " << (s1 ? "非空" : "空") << "\n";

        // 移动构造: 所有权从 s1 转移到 s2
        my_unique_ptr<Student> s2 = std::move(s1);

        std::cout << "  移动后: s1 " << (s1 ? "非空" : "空") << "\n";
        std::cout << "  移动后: s2 " << (s2 ? "非空" : "空") << "\n";
        s2->introduce();

        std::cout << "  同一时刻只有一个人拥有这个 Student\n";
    } // <-- s2 析构, Student 被正确回收; s1 是空, 析构无操作
    std::cout << "\n";

    // ============================================================
    std::cout << "========== 4. RAII 总结 ==========\n";
    std::cout <<
        "  RAII (Resource Acquisition Is Initialization)\n"
        "  ┌─────────────────────────────────────────────┐\n"
        "  │ 构造时  = 获取资源 (初始化即获取)            │\n"
        "  │ 析构时  = 释放资源 (离开作用域自动触发)       │\n"
        "  │ =delete = 禁止拷贝, 保证独占语义             │\n"
        "  │ move    = 转移所有权, 灵活而安全              │\n"
        "  └─────────────────────────────────────────────┘\n"
        "  无论函数如何返回 (正常/异常), 栈上对象都会析构,\n"
        "  所以 RAII 天然异常安全, 不会泄漏。\n";

    return 0;
}
