
#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>

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

public slots:
    void startGame();
    void runAway();
    void selectBattle();
    void selectPokemon();
    void selectBag();

private:
    Ui::GameMainWindow *ui;

};

#endif // GAMEMAINWINDOW_H
