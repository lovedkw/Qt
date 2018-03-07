#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTime>
#include <QStatusBar>

#define GAME_NAME                 QString("贪吃蛇")             // 游戏名称
#define CN_OK                     QString("确定")               // Ok 的中文释义
#define CN_CANCEL                 QString("取消")               // Cancel 的中文释义
#define CN_SCORE                  QString("您输了，总得分：%1 ！") // 得分的确认文字
#define CN_EXIT_GAME              QString("您确定要退出游戏吗？")  // 退出游戏的确认文字
#define CN_GAME_EXPLAIN           QString("上下左右、WSAD 控制蛇头方向，怎么这么笨的人还来玩游戏！！！")
#define CN_ABOUT_AUTHOR           QString("丁可炜，男，爱好女，就这么 easy！")

#define DIR_UP                    1                            // 上
#define DIR_DOWN                  2                            // 下
#define DIR_LEFT                  3                            // 左
#define DIR_RIGHT                 4                            // 右

#define CIRCLE_DIAMETER           8                            // 蛇身每一小节和食物的直径
#define CIRCLE_RADIUS             4                            // 蛇身每一小节和食物的半径

#define INIT_BODY_LEN             10                           // 初始化的蛇身长度

#define FIRST_LEVEL               50                           // 游戏难度 1 级，刷新频率为 50 毫秒
#define SECOND_LEVEL              20                           // 游戏难度 2 级，刷新频率为 20 毫秒
#define THIRD_LEVEL               10                           // 游戏难度 3 级，刷新频率为 10 毫秒

#define SECOND_LEVEL_INTERVAL     60000                        // 游戏难度提升为 2 级，时间间隔为 1 分钟
#define THIRD_LEVEL_INTERVAL      180000                       // 游戏难度提升为 3 级，时间间隔为 3 分钟

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_restart_triggered();

    void on_exit_triggered();

    void on_gameexplain_triggered();

    void on_aboutauthor_triggered();

private:
    Ui::MainWindow *ui;

    // 方向：1 - 上、2 - 下、3 - 左、4 - 右（volatile 可在很大程度上保持数据的一致性）
    volatile int m_nDirection;

    // 定时器 ID
    int m_nTimerID;

    // 食物所在位置
    QPoint m_aFood;

    // 是否已经吃了食物（volatile 可在很大程度上保持数据的一致性）
    volatile bool m_bEat;

    // 吃到的食物数量（volatile 可在很大程度上保持数据的一致性）
    volatile int m_nEatNum;

    // 组成蛇身体的结点动态数组
    std::vector<QPoint> m_vtPoint;

    // 游戏计时，游戏难度会随游戏时间的增加而增加
    QTime m_aTime;

    // 绘图事件
    void paintEvent(QPaintEvent *event);

    // 定时事件
    void timerEvent(QTimerEvent *event);

    // 按键事件
    void keyPressEvent(QKeyEvent *event);

    // 关闭事件
    void closeEvent(QCloseEvent *event);

    // 开始游戏
    void StartGame();

    // 画食物
    void DrawFood();

    // 画蛇身
    void DrawBody();
};

#endif // MAINWINDOW_H
