#include <iostream>

// 错误示范（取消注释会编译失败）：
// void badPrint(void* data) {
//     std::cout << *data << std::endl;  // error: 'void*' is not a pointer-to-object type
// }

// 回调示例：void* 传递用户数据
void printInt(void* data) {
    // 必须强转后才能使用
    int* p = static_cast<int*>(data);
    std::cout << "int: " << *p << std::endl;
}

void printDouble(void* data) {
    double* p = static_cast<double*>(data);
    std::cout << "double: " << *p << std::endl;
}

void printString(void* data) {
    char* p = static_cast<char*>(data);
    std::cout << "string: " << p << std::endl;
}

// 通用调用器：用 tag 区分类型
void callByTag(void* data, int tag) {
    switch (tag) {
        case 1: printInt(data);    break;
        case 2: printDouble(data); break;
        case 3: printString(data); break;
        default: std::cout << "unknown type" << std::endl;
    }
}

int main() {
    int    n = 42;
    double d = 3.14;
    char   s[] = "hello";

    // void* 可以接收任意类型的地址
    callByTag(&n, 1);
    callByTag(&d, 2);
    callByTag(s,  3);

    return 0;
}
