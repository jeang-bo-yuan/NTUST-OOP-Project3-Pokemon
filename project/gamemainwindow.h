
#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class GameMainWindow; }
QT_END_NAMESPACE

class GameMainWindow : public QMainWindow

{
    Q_OBJECT

public:
    GameMainWindow(QWidget *parent = nullptr);
    ~GameMainWindow();

private:
    Ui::GameMainWindow *ui;
};

#endif // GAMEMAINWINDOW_H
