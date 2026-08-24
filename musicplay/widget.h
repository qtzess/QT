// 头文件保护宏，防止同一个头文件被重复包含，避免重复定义错误。
#ifndef WIDGET_H
#define WIDGET_H

// QWidget 是 Qt Widgets 中所有普通窗口/控件的基类之一。
// 我们的 Widget 类会继承它，从而拥有窗口的基本能力。
#include <QWidget>
#include <QMediaPlayer>

// Ui 命名空间中的 Widget 类由 Qt 根据 widget.ui 自动生成。
// 这里先做前向声明，避免在头文件里直接包含自动生成的 ui_widget.h。
namespace Ui {
class Widget;
}

// 自定义窗口类。
// 继承 QWidget，表示它本身就是一个可以显示的窗口/控件。
class Widget : public QWidget
{
    // Qt 元对象系统宏。
    // 使用信号与槽、属性、tr() 翻译等 Qt 特性时需要它。
    Q_OBJECT

public:
    // 构造函数。
    // explicit 防止 QWidget* 被隐式转换成 Widget。
    // parent 表示父窗口/父控件，默认为 nullptr，说明它是顶层窗口。
    explicit Widget(QWidget *parent = nullptr);
    void initMusicTotal();

    // 析构函数。
    // 窗口销毁时会调用，用来释放资源。
    ~Widget();

private slots:
    // 播放按钮被点击时触发。
    // 如果当前正在播放，就暂停；否则开始/继续播放。
    void onPlayClicked();

    // 停止按钮被点击时触发。
    // 停止音乐，并把播放位置回到开头。
    void onStopClicked();

    // 音乐总时长变化时触发。
    // duration 是 QMediaPlayer 传来的总时长，单位是毫秒。
    void initTotalTime(qint64 duration);

    // 音乐播放位置变化时触发。
    // position 是当前播放位置，单位是毫秒。
    void updatePlayPosition(qint64 position);

    // 拖动滑轨时触发。
    // position 是滑轨当前值，对应播放器的毫秒位置。
    void onSliderMoved(int position);

    void playMusic();

private:
    // 根据播放器状态刷新播放按钮的文字和图标。
    void updatePlayButton();

    // 指向界面对象的指针。
    // setupUi(this) 会根据 widget.ui 创建控件并绑定到这个 ui 对象上。
    Ui::Widget *ui;
    QString musicDir;

    // QMediaPlayer 是 Qt Multimedia 提供的音频/视频播放类。
    QMediaPlayer *player;
};

// 结束头文件保护宏。
#endif // WIDGET_H
