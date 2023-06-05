/**************************************************//**
 * \author 蔣博元
 * \date 2023/05/29
 *****************************************************/
#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>

#include "Game.h"

#define gameWindow GameMainWindow::singleton()

QT_BEGIN_NAMESPACE
namespace Ui { class GameMainWindow; }
QT_END_NAMESPACE

class GameMainWindow : public QMainWindow

{
    Q_OBJECT

    GameMainWindow(QWidget *parent = nullptr);
    GameMainWindow(const GameMainWindow&) = delete;
    GameMainWindow(GameMainWindow&&) = delete;
    ~GameMainWindow();

    static GameMainWindow* singleWindow;  //!< 指向唯一的GameMainWindow
public:
    // Intend: 存取唯一的GameMainWindow
    // Pre: none
    // Post: 若第一次呼叫，分配記憶體並回傳指標，否則直接回傳指標
    static GameMainWindow* singleton();

    // Intend: 繪製背景
    // Pre: 收到paint event
    // Post: 將背景圖填滿整個視窗
    void paintEvent(QPaintEvent*) override;

private slots:
    // 開始遊戲
    void startGame();
    // 逃跑
    void runAway();
    // 顯示battle選單
    void selectBattle();
    // 顯示pokemon選單
    void selectPokemon();
    // 顯示bag選單
    void selectBag();
    // 顯示log window
    void selectLogWindow();

    // 取消選取戰鬥選單（將按鈕設成uncheck）
    void uncheckOptions();

private:
    Ui::GameMainWindow *ui;
    QImage backgroundImg;
    Game gameManager;
};

#endif // GAMEMAINWINDOW_H
