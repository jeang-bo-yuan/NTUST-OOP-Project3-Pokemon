/************************************************************
 * \author 蔣博元
 ***********************************************************/
#ifndef BAGSELECTER_H
#define BAGSELECTER_H

#include <QScrollArea>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

#include "Player.h"

/**
 * @brief 選擇Item用的按鈕
 */
class ItemButton : public QFrame {
    Q_OBJECT

    int index;
    int count;
    QLabel* itemCount;

public:
    ItemButton(Object* object, int index, QWidget* parent = nullptr);

    void useOne();

    int getIndex() const { return index; }

protected:
    // Intend: 被點擊時，看道具可不可以選，若可以則emit signal
    // Pre: count > 0 才可選
    // Post: 選擇成功，--count
    void mousePressEvent(QMouseEvent*) override;

    // Intend: 顯示邊框
    // Pre: none
    // Post: 鼠標進入->顯示，鼠標移出->隱藏
    void enterEvent(QEvent*) override { setFrameStyle(QFrame::Panel); }
    void leaveEvent(QEvent*) override { setFrameStyle(QFrame::NoFrame); }

signals:
    // Intend: 被選中時發出
    // Pre: none
    // Post: none
    void itemSelected(ItemButton* button);
};

/**
 * @brief 一個面版，顯示可選的item
 */
class BagSelecter : public QScrollArea
{
    Q_OBJECT

public:
    // ctor
    explicit BagSelecter(QWidget* parent=nullptr);

public slots:
    // Intend: 將BagSelecter的內容初始化成player的objects
    // Pre: player != nullptr
    // Post: 在bagSlots中加上按鈕
    void init(const Player* player);

signals:
    // Intend: 有道具被選中時發出
    // Pre: none
    // Post: 指標
    void itemSelected(ItemButton* button);

private:
    QGridLayout* bagSlots;
};

#endif // BAGSELECTER_H
