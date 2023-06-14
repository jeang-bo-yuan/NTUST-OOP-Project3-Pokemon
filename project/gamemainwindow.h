/**************************************************//**
 * \author 蔣博元
 * \date 2023/05/29
 *****************************************************/
#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>

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

    // Intend: constructor
    // Pre: none
    // Post: 建立預設外觀和connect signal and slot
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

private:
    // Intend: 選擇pokemon
    // Pre: allowNull -> 是否顯示back按鈕。
    // Post: 回傳index。     -1 -> 按下back。
    // 只有血量大於0的pokemon可以被選取，當pokemon全部血量歸零且allowNull == false時，會卡在函式內的event loop。
    int choosePokemon(bool allowNull = false);

private slots:
    // Intend: 開始遊戲
    // Pre: 按下"start game"
    // Post: 對遊戲初始化
    void startGame();
    // Intend: 逃跑
    // Pre: 按下"run"
    // Post: 逃跑並backToMain
    void runAway();
    // Intend: 顯示battle選單, index = 1
    // Pre: 按下"Battle"
    // Post: 第一次按下是開啟，第二次是關閉
    void selectBattle();
    // Intend: 顯示pokemon選單, index = 2
    // Pre: 按下"pokemon"
    // Post: 第一次按是開啟，第二次是關閉
    void selectPokemon();
    // Intend: 顯示bag選單, index = 3
    // Pre: 按下"Bag"
    // Post: 第一次點是開啟，第二次是關閉
    void selectBag();
    // Intend: 顯示log window, index = 0
    // Pre: none
    // Post: 顯示log window
    void selectLogWindow();

    // Intend: 取消選取戰鬥選單（將按鈕設成uncheck）
    // Pre: none
    // Post: 讓戰鬥選單呈現unchecked，若pokemonSelecter是開啟的則reject它
    void uncheckOptions();

    // Intend: 選道具並讓對手攻擊
    // Pre: 選"bag" -> 選定道具
    // Post: 先選用在哪隻寶可夢上，若不是選「返回」，則讓對手攻擊並呼叫nextRound
    void itemSelected(ItemButton* button);
    // Intend: 選技能並讓雙方輪流攻擊
    // Pre: 選"battle" -> 選定技能
    // Post: 所有行動結束後呼叫nextRound
    void skillSelected(SkillButton* button);
    // Intend: 選好要交換的寶可夢並讓對手攻擊
    // Pre: 選"pokemon" -> 選定寶可夢
    // Post: 所有行動結束後呼叫nextRound
    void pokemonSelected(PokemonButton* button);

    // Intend: 遊戲進入下一回合，包含判斷遊戲是否結束
    // Pre: 玩家選好要幹嘛 + 電腦行動完
    // Post: 進入 B&P 接著檢查遊戲是否結束 並將fainted的寶可夢換下場
    void nextRound();

    // Intend: 返回主畫面
    // Pre: none
    // Post: uncheckOptions + 切回主畫面及log + 關掉bgm
    void backToMain();

private:
    Ui::GameMainWindow *ui;
    QImage backgroundImg;
    Game gameManager;
    Player* player = nullptr;
    Player* computer = nullptr;

    QMediaPlayer bgm;
};

#endif // GAMEMAINWINDOW_H
