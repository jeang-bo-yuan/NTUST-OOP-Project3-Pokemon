/**************************************************//**
 * \author 蔣博元
 * \date 2023/05/29
 *****************************************************/
#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>

#include "BagSelecter.h"
#include "PokemonSelecter.h"
#include "SkillSelecter.h"
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

    // Intend: 選擇pokemon
    // Pre: allowNull -> 是否顯示back按鈕。
    // Post: 回傳index。     -1 -> 按下back。
    // 只有血量大於0的pokemon可以被選取，當pokemon全部血量歸零且allowNull == false時，會卡在函式內的event loop。
    int choosePokemon(bool allowNull = false);

private slots:
    // 開始遊戲
    void startGame();
    // 逃跑
    void runAway();
    // 顯示battle選單, index = 1
    void selectBattle();
    // 顯示pokemon選單, index = 2
    void selectPokemon();
    // 顯示bag選單, index = 3
    void selectBag();
    // 顯示log window, index = 0
    void selectLogWindow();

    // 取消選取戰鬥選單（將按鈕設成uncheck）
    void uncheckOptions();

    // 選bag -> 選定道具
    void itemSelected(ItemButton* button);
    // 選battle -> 選定技能
    void skillSelected(SkillButton* button);
    // 選pokemon -> 選定寶可夢
    void pokemonSelected(PokemonButton* button);

    // 遊戲進入下一回合，包含判斷遊戲是否結束
    void nextRound();

private:
    Ui::GameMainWindow *ui;
    QImage backgroundImg;
    Game gameManager;
    Player* player = nullptr;
    Player* computer = nullptr;
};

#endif // GAMEMAINWINDOW_H
