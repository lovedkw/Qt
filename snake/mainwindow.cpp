#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 开始游戏
    StartGame(false);
}

MainWindow::~MainWindow()
{
    killTimer(m_nTimerID);

    delete ui;
}

// 绘图事件
void MainWindow::paintEvent(QPaintEvent *event)
{
    // 画食物
    DrawFood();

    // 画蛇身
    DrawBody();

    return;
}

// 定时事件
void MainWindow::timerEvent(QTimerEvent *event)
{
    // 记录蛇尾索引
    int nHigh = m_vtPoint.size() - 1;

    // 判断蛇头是否与蛇身碰撞
    bool bGameOver = false;
    for (int i = 1; i <= nHigh; ++i)
        if (m_vtPoint[0] == m_vtPoint[i])
            bGameOver = true;

    // 若蛇头与蛇身或边界碰撞，则游戏结束
    if (
            bGameOver ||
            m_vtPoint[0].x() <= 0 ||
            m_vtPoint[0].x() >= width() - 10 ||
            m_vtPoint[0].y() <= 20 ||
            m_vtPoint[0].y() >= height() - 10
            )
    {
        // 结束定时器
        killTimer(m_nTimerID);

        // 弹出游戏结束对话框
        QMessageBox aMB(QMessageBox::Information, GAME_NAME, CN_SCORE.arg(m_nEatNum));
        aMB.setStandardButtons(QMessageBox::Ok);
        aMB.setButtonText(QMessageBox::Ok, CN_OK);
        aMB.exec();

        return;
    }

    // 蛇头与食物圆心横坐标之差的绝对值与纵坐标之差的绝对值均小于等于直径时，表示蛇头已触及该食物
    if (
            abs(m_vtPoint[0].x() - m_aFood.x()) <= CIRCLE_DIAMETER &&
            abs(m_vtPoint[0].y() - m_aFood.y()) <= CIRCLE_DIAMETER
            )
    {
        ++m_nEatNum;
        m_bEat = true;
    }

    // 根据移动方向判断下一步该如何绘图
    switch (m_nDirection)
    {
    case DIR_UP:
        // 先循环将蛇身都往前移动一个位置（包括蛇头）
        for (int i = nHigh; i > 0; --i)
        {
            m_vtPoint[i].setX(m_vtPoint[i - 1].x());
            m_vtPoint[i].setY(m_vtPoint[i - 1].y());
        }

        // 然后再根据移动方向调整蛇头的位置
        m_vtPoint[0].setY(m_vtPoint[0].y() - CIRCLE_RADIUS);

        // 若已吃了食物，则蛇身长度加 1
        if (m_bEat)
            m_vtPoint.push_back(QPoint(m_vtPoint[nHigh].x(), m_vtPoint[nHigh].y() + CIRCLE_RADIUS));

        break;
    case DIR_DOWN:
        // 先循环将蛇身都往前移动一个位置（包括蛇头）
        for (int i = nHigh; i > 0; --i)
        {
            m_vtPoint[i].setX(m_vtPoint[i - 1].x());
            m_vtPoint[i].setY(m_vtPoint[i - 1].y());
        }

        // 然后再根据移动方向调整蛇头的位置
        m_vtPoint[0].setY(m_vtPoint[0].y() + CIRCLE_RADIUS);

        // 若已吃了食物，则蛇身长度加 1
        if (m_bEat)
            m_vtPoint.push_back(QPoint(m_vtPoint[nHigh].x(), m_vtPoint[nHigh].y() - CIRCLE_RADIUS));

        break;
    case DIR_LEFT:
        // 先循环将蛇身都往前移动一个位置（包括蛇头）
        for (int i = nHigh; i > 0; --i)
        {
            m_vtPoint[i].setX(m_vtPoint[i - 1].x());
            m_vtPoint[i].setY(m_vtPoint[i - 1].y());
        }

        // 然后再根据移动方向调整蛇头的位置
        m_vtPoint[0].setX(m_vtPoint[0].x() - CIRCLE_RADIUS);

        // 若已吃了食物，则蛇身长度加 1
        if (m_bEat)
            m_vtPoint.push_back(QPoint(m_vtPoint[nHigh].x() + CIRCLE_RADIUS, m_vtPoint[nHigh].y()));

        break;
    case DIR_RIGHT:
        // 先循环将蛇身都往前移动一个位置（包括蛇头）
        for (int i = nHigh; i > 0; --i)
        {
            m_vtPoint[i].setX(m_vtPoint[i - 1].x());
            m_vtPoint[i].setY(m_vtPoint[i - 1].y());
        }

        // 然后再根据移动方向调整蛇头的位置
        m_vtPoint[0].setX(m_vtPoint[0].x() + CIRCLE_RADIUS);

        // 若已吃了食物，则蛇身长度加 1
        if (m_bEat)
            m_vtPoint.push_back(QPoint(m_vtPoint[nHigh].x() - CIRCLE_RADIUS, m_vtPoint[nHigh].y()));

        break;
    default:
        break;
    }

    // 更新界面
    update();

    // 判断是否需要提升游戏难度
    int nElapsed = m_aTime.elapsed();
    if (nElapsed >= THIRD_LEVEL_INTERVAL && SECOND_LEVEL == m_nLevel)
    {
        killTimer(m_nTimerID);
        m_nTimerID = startTimer(THIRD_LEVEL);

        m_nLevel = THIRD_LEVEL;
    }
    else if (nElapsed >= SECOND_LEVEL_INTERVAL && FIRST_LEVEL == m_nLevel)
    {
        killTimer(m_nTimerID);
        m_nTimerID = startTimer(SECOND_LEVEL);

        m_nLevel = SECOND_LEVEL;
    }

    return;
}

// 按键事件
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
    case Qt::Key_W:
        // 不能直接将蛇头反向啊！！！不然不是把自己给吃了
        if (DIR_DOWN == m_nDirection)
            break;

        // 改变移动方向
        m_nDirection = DIR_UP;

        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        // 不能直接将蛇头反向啊！！！不然不是把自己给吃了
        if (DIR_UP == m_nDirection)
            break;

        // 改变移动方向
        m_nDirection = DIR_DOWN;

        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        // 不能直接将蛇头反向啊！！！不然不是把自己给吃了
        if (DIR_RIGHT == m_nDirection)
            break;

        // 改变移动方向
        m_nDirection = DIR_LEFT;

        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        // 不能直接将蛇头反向啊！！！不然不是把自己给吃了
        if (DIR_LEFT == m_nDirection)
            break;

        // 改变移动方向
        m_nDirection = DIR_RIGHT;

        break;
    default:
        break;
    }

    return;
}

// 关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox aMB(QMessageBox::Question, GAME_NAME, CN_EXIT_GAME);
    aMB.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    aMB.setButtonText(QMessageBox::Ok, CN_OK);
    aMB.setButtonText(QMessageBox::Cancel, CN_CANCEL);
    if (QMessageBox::Ok != aMB.exec())
        event->ignore();

    return;
}

// 开始游戏
void MainWindow::StartGame(bool bRestart)
{
    // 初始化蛇身
    m_vtPoint.clear();
    for (int i = 0; i < INIT_BODY_LEN; ++i)
        m_vtPoint.push_back(QPoint(width() / 2 + CIRCLE_RADIUS * i, height() / 2));

    // 是否已经吃了食物，初始化为 true
    m_bEat = true;

    // 吃到的食物数量，初始化为 0
    m_nEatNum = 0;

    // 默认移动方向为 3 - 左
    m_nDirection = DIR_LEFT;

    // 当前游戏难度，默认为 1 级
    m_nLevel = FIRST_LEVEL;

    // 开启游戏计时
    m_aTime.restart();

    // 启动定时器，初始化游戏难度为 1 级
    if (bRestart)
        killTimer(m_nTimerID);
    m_nTimerID = startTimer(FIRST_LEVEL);

    return;
}

// 画食物
void MainWindow::DrawFood()
{
    // 若该食物已被吃，则重新随机生成一个食物
    if (m_bEat)
    {
        // 生成随机数种子
        QTime aTime = QTime::currentTime();
        qsrand(aTime.msec() * qrand() * qrand());

        // 先尝试通过取模得到食物的位置
        int nXPos = qrand() % width();
        int nYPos = qrand() % height();

        // 再根据一定规则调整食物的位置
        nXPos = nXPos <= 5 ? nXPos + 5 : nXPos;
        nXPos = nXPos >= width() - 15 ? nXPos - 15 : nXPos;
        nYPos = nYPos <= 25 ? nYPos + 25 : nYPos;
        nYPos = nYPos >= height() - 15 ? nYPos - 15 : nYPos;

        // 设置食物的位置
        m_aFood.setX(nXPos);
        m_aFood.setY(nYPos);

        // 表示该食物还没有被吃
        m_bEat = false;
    }

    // 画食物
    QPainter aPainter(this);
    aPainter.setBrush(Qt::SolidPattern);
    aPainter.drawEllipse(m_aFood, CIRCLE_RADIUS, CIRCLE_RADIUS);

    return;
}

// 画蛇身
void MainWindow::DrawBody()
{
    // 画蛇身
    QPainter aPainter(this);
    aPainter.setBrush(Qt::SolidPattern);
    for (uint i = 0; i < m_vtPoint.size(); ++i)
    {
        aPainter.drawEllipse(m_vtPoint[i].x(), m_vtPoint[i].y(), CIRCLE_DIAMETER, CIRCLE_DIAMETER);
    }

    return;
}

void MainWindow::on_restart_triggered()
{
    StartGame(true);

    return;
}

void MainWindow::on_exit_triggered()
{
    QMessageBox aMB(QMessageBox::Question, GAME_NAME, CN_EXIT_GAME);
    aMB.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    aMB.setButtonText(QMessageBox::Ok, CN_OK);
    aMB.setButtonText(QMessageBox::Cancel, CN_CANCEL);
    if (QMessageBox::Ok == aMB.exec())
        qApp->exit();

    return;
}

void MainWindow::on_gameexplain_triggered()
{
    QMessageBox aMB(QMessageBox::Information, GAME_NAME, CN_GAME_EXPLAIN);
    aMB.setStandardButtons(QMessageBox::Ok);
    aMB.setButtonText(QMessageBox::Ok, CN_OK);
    aMB.exec();

    return;
}

void MainWindow::on_aboutauthor_triggered()
{
    QMessageBox aMB(QMessageBox::Information, GAME_NAME, CN_ABOUT_AUTHOR);
    aMB.setStandardButtons(QMessageBox::Ok);
    aMB.setButtonText(QMessageBox::Ok, CN_OK);
    aMB.exec();

    return;
}
