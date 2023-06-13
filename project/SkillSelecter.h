/************************************************************
 * \author 蔣博元
 ***********************************************************/
#ifndef SKILLSELECTER_H
#define SKILLSELECTER_H

#include <QScrollArea>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include "Creature.h"

/**
 * @brief 選擇技能的按鈕
 */
class SkillButton : public QFrame {
    Q_OBJECT

    int index;
    int pp;
    QLabel* skillPP;
    const Skill& skill;

public:
    SkillButton(const Skill& theSkill, int index, QWidget* parent = nullptr);

    int getIndex() const { return index; }

signals:
    void skillSelected(SkillButton* button);

protected:
    void enterEvent(QEvent*) override { setFrameStyle(QFrame::Panel); }
    void leaveEvent(QEvent*) override { setFrameStyle(QFrame::NoFrame); }

    // Intend: mouse click
    // Pre: click left mouse && pp > 0
    // Post: emit skillSelected(this) and update pp information
    void mousePressEvent(QMouseEvent*) override;
};

/**
 * @brief 選擇skill的面版
 */
class SkillSelecter : public QScrollArea {
    Q_OBJECT

    QGridLayout* skillSlots;

public:
    explicit SkillSelecter(QWidget* parent = nullptr);

    // Intend: 將面版上的按鈕設成creature的技能
    // Pre: creature != nullptr
    // Post: 建立按鈕
    void init(Creature* creature);

signals:
    void skillSelected(SkillButton*);
};

#endif // SKILLSELECTER_H
