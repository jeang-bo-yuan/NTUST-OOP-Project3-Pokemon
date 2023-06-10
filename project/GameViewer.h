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
#include "Player.h"


class GameViewer : public QWidget
{
    Q_OBJECT

    QHBoxLayout* pokeBallBox;
    QLabel* name;
    QProgressBar* hpBar;
    QLabel* pokemonImg;

    void setData(Player* player);

public:
    explicit GameViewer(QWidget *parent = nullptr);

public slots:
    // 初始化顯示
    void init(Player* player);

    // 更新hp+異常狀態
    void updateHp(Player* player);

    // 更新pokemon+顯示的name, hp
    void switchPokemon(Player* player);

signals:

};

#endif // GAMEVIEWER_H
