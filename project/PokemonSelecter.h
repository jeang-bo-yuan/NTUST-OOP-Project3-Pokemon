
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

    Creature* pokemonPtr;
    int index;
    QLabel* pokemonHp;

public:
    PokemonButton(Creature* pokemon, int index, QWidget* parent = nullptr);

    int getIndex() const { return index; }

public slots:
    // 更新顯示的Hp
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
    PokemonSelecter(QWidget* parent = nullptr);

    // Intend: 依player擁有的pokemon初始化按鈕
    // Pre: player != nullptr
    // Post: 在scroll area 內建立按鈕
    void init(Player* player);

    void setAllowNull(bool allowNull) { buttonBack->setVisible(allowNull); }
signals:
    // emit when a pokemon is selected
    // nullptr, if "back" is clicked
    void pokemonSelected(PokemonButton* button);
};

#endif // POKEMONSELECTER_H
