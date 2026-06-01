#include <iostream>
#include <cstring>
#include <cstdint>

// ============================================================
// 主模板：正常的 Vector<T>
// ============================================================
template <typename T>
class Vector {
    T*      _data;
    size_t  _size;
public:
    Vector() : _data(nullptr), _size(0) {}

    void push_back(const T& val) {
        T* tmp = new T[_size + 1];
        for (size_t i = 0; i < _size; ++i)
            tmp[i] = _data[i];
        tmp[_size] = val;
        delete[] _data;
        _data = tmp;
        ++_size;
    }

    T get(size_t i) const { return _data[i]; }
    size_t size()   const { return _size; }
    size_t memory() const { return _size * sizeof(T); }  // 占用字节数
};

// ============================================================
// 全特化：Vector<bool>  ——  位压缩存储
// 1 byte 塞 8 个 bool，而不是 1 个 bool 占 1 byte
// ============================================================
template <>
class Vector<bool> {
    uint8_t* _bits;   // 用字节数组存位
    size_t   _size;
public:
    Vector() : _bits(nullptr), _size(0) {}

    void push_back(bool val) {
        // 需要多少字节来存 _size+1 个 bit
        size_t new_bytes = (_size + 1 + 7) / 8;
        size_t old_bytes = (_size + 7) / 8;

        uint8_t* tmp = new uint8_t[new_bytes];
        std::memset(tmp, 0, new_bytes);

        // 拷贝旧的位数据
        if (_bits) {
            for (size_t i = 0; i < old_bytes; ++i)
                tmp[i] = _bits[i];
            delete[] _bits;
        }

        _bits = tmp;
        // 设置新位
        if (val) {
            size_t byte_idx = _size / 8;   // 第几个字节
            size_t bit_idx  = _size % 8;   // 该字节的第几位
            _bits[byte_idx] |= (1 << bit_idx);
        }
        ++_size;
    }

    bool get(size_t i) const {
        size_t byte_idx = i / 8;
        size_t bit_idx  = i % 8;
        return (_bits[byte_idx] >> bit_idx) & 1;
    }

    size_t size()   const { return _size; }
    size_t memory() const { return (_size + 7) / 8; }
};

// ============================================================
// 偏特化举例：Vector<T*>  ——  指针版本（可以做深拷贝/打印地址等）
// ============================================================
template <typename T>
class Vector<T*> {
    T**     _data;
    size_t  _size;
public:
    Vector() : _data(nullptr), _size(0) {}

    void push_back(T* val) {
        T** tmp = new T*[_size + 1];
        for (size_t i = 0; i < _size; ++i)
            tmp[i] = _data[i];
        tmp[_size] = val;
        delete[] _data;
        _data = tmp;
        ++_size;
    }

    T* get(size_t i) const { return _data[i]; }
    size_t size()   const { return _size; }
    size_t memory() const { return _size * sizeof(T*); }

    void info() const {
        std::cout << "[偏特化 Vector<T*>]: 存的是指针" << std::endl;
    }
};

// 偏特化：Vector<const T*>  ——  另一种指针模式
template <typename T>
class Vector<const T*> {
    const T** _data;
    size_t    _size;
public:
    Vector() : _data(nullptr), _size(0) {}

    void push_back(const T* val) {
        const T** tmp = new const T*[_size + 1];
        for (size_t i = 0; i < _size; ++i)
            tmp[i] = _data[i];
        tmp[_size] = val;
        delete[] _data;
        _data = tmp;
        ++_size;
    }

    const T* get(size_t i) const { return _data[i]; }
    size_t size()   const { return _size; }

    void info() const {
        std::cout << "[偏特化 Vector<const T*>]: 存的是 const 指针" << std::endl;
    }
};

// ============================================================
// main
// ============================================================
int main() {
    // ---- 主模板 Vector<int> ----
    Vector<int> vi;
    for (int i = 1; i <= 10; ++i) vi.push_back(i);
    std::cout << "Vector<int>  : size=" << vi.size()
              << ", 内存=" << vi.memory() << " bytes" << std::endl;

    // ---- 全特化 Vector<bool> 位压缩 ----
    Vector<bool> vb;
    for (int i = 0; i < 10; ++i) vb.push_back(i % 2 == 1); // true,false 交替
    std::cout << "Vector<bool> : size=" << vb.size()
              << ", 内存=" << vb.memory() << " bytes (位压缩)" << std::endl;

    std::cout << "vb: ";
    for (size_t i = 0; i < vb.size(); ++i)
        std::cout << vb.get(i) << " ";
    std::cout << std::endl;

    // ---- 对比：如果 bool 也用主模板，10 个 bool = 10 bytes ----
    // 全特化后 10 个 bool = 2 bytes
    std::cout << "\n节省: 主模板 10 bytes -> 全特化 2 bytes" << std::endl;

    // ---- 偏特化 Vector<int*> ----
    int a = 100, b = 200;
    Vector<int*> vp;
    vp.push_back(&a);
    vp.push_back(&b);
    vp.info();
    std::cout << "  vp.get(0) = " << *vp.get(0) << std::endl;

    // ---- 偏特化 Vector<const char*> ----
    Vector<const char*> vs;
    vs.push_back("hello");
    vs.push_back("world");
    vs.info();
    std::cout << "  vs.get(0) = " << vs.get(0) << std::endl;

    return 0;
}
