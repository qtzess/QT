// 引入 Widget 类自己的声明。
#include "widget.h"

// 引入由 Qt 的 uic 工具根据 widget.ui 自动生成的界面头文件。
// 里面包含 Ui::Widget 类和 setupUi() 函数。
#include "ui_widget.h"
#include <QIcon>
#include <QMediaPlayer>
#include <QPushButton>
#include <QStyle>
#include <QUrl>
#include <QString>
#include <QSignalBlocker>
#include <QSlider>
#include <QDir>

// 把毫秒转换成“分钟:秒”的显示格式，例如 65000 -> 1:05。
static QString formatTime(qint64 milliseconds)
{
    const qint64 totalSeconds = milliseconds / 1000;
    const qint64 minutes = totalSeconds / 60;
    const qint64 seconds = totalSeconds % 60;

    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

// Widget 构造函数的实现。
// parent 会传给 QWidget，让 Qt 建立父子对象关系。
Widget::Widget(QWidget *parent) :
    // 调用父类 QWidget 的构造函数。
    QWidget(parent),
    // 创建 Ui::Widget 对象，用来管理 .ui 文件中设计的界面控件。
    ui(new Ui::Widget),
    // 创建播放器对象，并把当前 Widget 作为父对象。
    // 这样 Widget 销毁时，Qt 会自动释放 player。
    player(new QMediaPlayer(this))
{
    // 按照 widget.ui 中的设计初始化当前窗口。
    // this 表示把这些控件安装到当前 Widget 窗口上。
    ui->setupUi(this);
    setWindowTitle("音乐播放器");
    setWindowIcon(QIcon(":/image/cat.png"));

    // 设置要播放的本地音乐文件。
    // fromLocalFile 会把 Windows 本地路径转换成 QMediaPlayer 能识别的 file:/// URL。


    initMusicTotal();
    // 初始显示时间和滑轨。
    ui->playtime->setReadOnly(true);
    ui->totiltime->setReadOnly(true);
    ui->playtime->setText(formatTime(0));
    ui->totiltime->setText(formatTime(0));
    ui->playslider->setRange(0, 0);
    ui->playslider->setValue(0);



    // clicked 是按钮被鼠标点击时发出的信号。
    // connect 把这个信号和我们自己写的槽函数连接起来。
    connect(ui->play, &QPushButton::clicked, this, &Widget::onPlayClicked);
    connect(ui->stop, &QPushButton::clicked, this, &Widget::onStopClicked);
    // durationChanged 是播放器 player 发出的信号，不是按钮 ui->play 发出的信号。
    // 当音乐文件加载出总时长后，Qt 会触发这个信号。
    connect(player, &QMediaPlayer::durationChanged, this, &Widget::initTotalTime);

    // positionChanged 会在播放进度变化时不断触发。
    // 用它来让滑轨和当前时间跟着音乐播放自动变化。
    connect(player, &QMediaPlayer::positionChanged, this, &Widget::updatePlayPosition);

    // sliderMoved 是用户拖动滑轨时发出的信号。
    // 用它来把播放器跳转到用户拖动到的位置。
    connect(ui->playslider, &QSlider::sliderMoved, this, &Widget::onSliderMoved);

    // 播放器状态变化时，也刷新按钮显示。
    // 例如 play() 后变成 PlayingState，pause() 后变成 PausedState。
    connect(player, &QMediaPlayer::stateChanged, this, &Widget::updatePlayButton);
    connect(ui->listWidget,&QListWidget::doubleClicked,this,&Widget::playMusic);

    // 初始状态是未播放，所以按钮显示“播放”和播放图标。
    updatePlayButton();
}

void Widget::initMusicTotal()
{
    musicDir = "C:\\Users\\72797\\Desktop\\QT\\musicplay\\music";//获取文件夹
    QDir dir(musicDir);
    if(dir.exists() == true)
    {

        QStringList musicList=dir.entryList(QDir::Files);  //获取目录里的条目列表
        for(auto ite = musicList.begin();ite != musicList.end();ite++)
        {
            if((*ite).contains(".mp3") == true)
            {
                ui->listWidget->addItem(*ite);
            }

        }
    }

}

// Widget 析构函数的实现。
Widget::~Widget()
{
    // 释放构造函数中 new 出来的 ui 对象，避免内存泄漏。
    delete ui;
}

void Widget::onPlayClicked()
{
    if (player->state() == QMediaPlayer::PlayingState) {
        // 当前正在播放：点击后暂停。
        player->pause();
    } else {
        // 当前没有播放或已经暂停：点击后开始/继续播放。
        player->play();
    }

    updatePlayButton();
}

void Widget::onStopClicked()
{
    // stop() 会停止播放，并把状态切到 StoppedState。
    player->stop();

    // 把进度放回开头，方便下次从头播放。
    player->setPosition(0);
    updatePlayPosition(0);
    updatePlayButton();
}

void Widget::initTotalTime(qint64 duration)
{
   ui->totiltime->setText(formatTime(duration));//把毫秒数转换成播放器里常见的 分钟:秒 格式

   // QSlider 的范围使用 int；普通歌曲时长转成 int 是安全的。
   ui->playslider->setRange(0, static_cast<int>(duration));
}

void Widget::updatePlayPosition(qint64 position)
{
    ui->playtime->setText(formatTime(position));

    // 程序更新滑轨时临时阻止它发信号，避免和用户拖动逻辑互相影响。
    QSignalBlocker blocker(ui->playslider);
    ui->playslider->setValue(static_cast<int>(position));
}

void Widget::onSliderMoved(int position)
{
    // 用户拖动滑轨时，把播放器跳到对应位置。
    player->setPosition(position);
    updatePlayPosition(position);
}

void Widget::playMusic()
{
    auto musicName = ui->listWidget->currentItem()->text();

    player->setMedia(QUrl::fromLocalFile(musicDir + "/" + musicName));
    player->play();
}

void Widget::updatePlayButton()
{
    const bool isPlaying = player->state() == QMediaPlayer::PlayingState;

    // 正在播放时，按钮显示“暂停”，表示下一次点击会暂停。
    // 没有播放时，按钮显示“播放”，表示下一次点击会播放。
    ui->play->setText(isPlaying ? "暂停" : "播放");
    ui->play->setIcon(QIcon(isPlaying ? ":/image/play.png" : ":/image/pause.png"));
}
