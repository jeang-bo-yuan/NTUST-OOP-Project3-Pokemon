/************************************//**
 * \author 蔣博元
****************************************/
#ifndef GAMEVIEWER_H
#define GAMEVIEWER_H


#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QSoundEffect>
#include "Player.h"


class GameViewer : public QWidget
{
    Q_OBJECT

    QHBoxLayout* pokeBallBox;
    QLabel* name;
    QLabel* condition;
    QProgressBar* hpBar;
    QLabel* pokemonImg;

    static QSoundEffect damageSound;
    static QSoundEffect healSound;

    // Intend: 設置顯示的內容（寶可夢圖片+hp+異常狀態+背包內存活的寶可夢）
    // Pre: none
    // Post: 顯示內容
    void setData(Player* player);

    // Intend: 顯示異常狀態
    // Pre: none
    // Post: 顯示文字
    void setCondition(Player* player);

public:
    explicit GameViewer(QWidget *parent = nullptr);

public slots:
    // Intend: 初始化顯示
    // Pre: none
    // Post: 重置pokeBallBox + call setData
    void init(Player* player);

    // Intend: 更新hp+異常狀態
    // Pre: none
    // Post: call setCondition + 以動畫顯示hp變動
    void updateHp(Player* player);

    // Intend: 更新pokemon img + 顯示的name, hp
    // Pre: none
    // Post: call setData
    void switchPokemon(Player* player);

signals:

};

#endif // GAMEVIEWER_H
