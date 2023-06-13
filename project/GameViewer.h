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
    QHBoxLayout* condition;
    QProgressBar* hpBar;
    QLabel* pokemonImg;

    static QSoundEffect damageSound;
    static QSoundEffect healSound;

    void setData(Player* player);

    void setCondition(Player* player);

public:
    explicit GameViewer(QWidget *parent = nullptr);

public slots:
    // 初始化顯示
    void init(Player* player);

    // 更新hp+異常狀態
    void updateHp(Player* player);

    // 更新pokemon img + 顯示的name, hp
    void switchPokemon(Player* player);

signals:

};

#endif // GAMEVIEWER_H
