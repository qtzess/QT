// 引入我们自己定义的 Widget 窗口类声明。
// Widget 是这个项目当前的主界面类。
#include "widget.h"

// QApplication 是 Qt Widgets 程序的应用对象。
// 它负责初始化 Qt、管理窗口、处理鼠标键盘等事件。
#include <QApplication>

// C/C++ 程序的入口函数。
// argc 和 argv 用来接收命令行参数，Qt 也会读取其中和界面相关的参数。
int main(int argc, char *argv[])
{
    // 创建 Qt 应用对象。
    // 一个 Qt Widgets 程序通常只能有一个 QApplication 对象。
    QApplication a(argc, argv);

    // 创建主窗口对象。
    // 这里的 Widget 类来自 widget.h / widget.cpp。
    Widget w;

    // 显示主窗口。
    // 如果不调用 show()，窗口对象虽然创建了，但不会出现在屏幕上。
    w.show();

    // 进入 Qt 的事件循环。
    // 程序会在这里持续等待和处理事件，例如点击按钮、拖动窗口、键盘输入等。
    // 当主窗口关闭或调用 quit() 时，exec() 返回，程序结束。
    return a.exec();
}
