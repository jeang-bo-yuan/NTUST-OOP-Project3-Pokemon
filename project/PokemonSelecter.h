/************************************************************
 * \author 蔣博元
 ***********************************************************/
#ifndef POKEMONSELECTER_H
#define POKEMONSELECTER_H

#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include "Player.h"

/**
 * @brief 選擇pokemon用的按鈕
 */
class PokemonButton : public QFrame {
    Q_OBJECT

    const Creature& pokemon;
    int index;
    QLabel* pokemonHp;

public:
    PokemonButton(const Creature& pokemon, int index, QWidget* parent = nullptr);

    int getIndex() const { return index; }

public slots:
    // Intend: 更新顯示的Hp
    // Pre: none
    // Post: 改pokemonHp的text
    void updateHp();

signals:
    void pokemonSelected(PokemonButton* button);

protected:
    // Intend: 顯示邊框
    // Pre: none
    // Post: 鼠標進入->顯示，鼠標移出->隱藏
    void enterEvent(QEvent*) override { setFrameStyle(QFrame::Panel | QFrame::Raised); }
    void leaveEvent(QEvent*) override { setFrameStyle(QFrame::Panel); }

    // Intend: 顯示時，更新hp
    // Pre: 顯示
    // Post: call updateHp
    void showEvent(QShowEvent*) override { updateHp(); }

    // Intend: 點擊事件
    // Pre: 點擊
    // Post: if hp > 0, emit pokemonSelected with `this` as parameter
    //       右鍵點擊，顯示pokemon資訊
    void mousePressEvent(QMouseEvent*) override;
};

/**
 * @brief 選pokemon的視窗，有返回鍵
 */
class PokemonSelecter : public QWidget {
    Q_OBJECT

    QGridLayout* pokemonSlots;
    QPushButton* buttonBack;

public:
    explicit PokemonSelecter(QWidget* parent = nullptr);

    // Intend: 依player擁有的pokemon初始化按鈕
    // Pre: player != nullptr
    // Post: 在scroll area 內建立按鈕
    void init(Player* player);

    // Intend: 改變是否顯示返回鍵
    // Pre: none
    // Post: allowNull為true顯示；否則隱藏
    void setAllowNull(bool allowNull) { buttonBack->setVisible(allowNull); }

public slots:
    // Intend: 效果等同按下"back"
    // Pre: none
    // Post: emit pokemonSelected(nullptr)
    void reject() { emit pokemonSelected(nullptr); }

signals:
    // emit when a pokemon is selected
    // nullptr, if "back" is clicked
    void pokemonSelected(PokemonButton* button);
};

#endif // POKEMONSELECTER_H
