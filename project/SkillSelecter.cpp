/************************************************************
 * \author 蔣博元
 ***********************************************************/
#include "SkillSelecter.h"
#include <QHBoxLayout>
#include <QMouseEvent>

// SkillButton

SkillButton::SkillButton(const skill& theSkill, int index, QWidget* parent)
    : QFrame(parent), index(index), pp(theSkill.pp), skillPP(new QLabel(QString::number(theSkill.pp)))
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // type icon
    {
        QLabel* type = new QLabel;
        QPixmap img(QString(":/media/types/") + theSkill.type.c_str());
        type->setPixmap(img.scaled(50, 50));
        mainLayout->addWidget(type);
    }

    // physical or special
    {
        QLabel* move_type = new QLabel;
        QPixmap img(QString(":/media/move-type/") + (theSkill.isPhysical ? "physical" : "special"));
        move_type->setPixmap(img.scaled(50, 50));
        mainLayout->addWidget(move_type);
    }

    // name
    {
        QLabel* name = new QLabel(theSkill.name.c_str());
        mainLayout->addWidget(name);
    }

    // pp
    {
        this->setDisabled(pp <= 0);
        mainLayout->addWidget(skillPP);
    }
}

void SkillButton::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton && pp > 0) {
        --pp;
        skillPP->setText(QString::number(pp));
        this->setDisabled(pp <= 0);
        emit skillSelected(this);
    }
}

// SkillSelecter

SkillSelecter::SkillSelecter(QWidget* parent)
    : QScrollArea(parent)
{
    QWidget* w = new QWidget;
    skillSlots = new QGridLayout(w);

    this->setWidget(w);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setWidgetResizable(true);
}

void SkillSelecter::init(Creature* creature) {
    // delete original skill buttons
    {
        QList<SkillButton*> list = findChildren<SkillButton*>();
        for (auto p : list) {
            skillSlots->removeWidget(p);
            delete p;
        }
    }

    // create buttons
    {
        for (int i = 0; i < creature->skills.size(); ++i) {
            SkillButton* button = new SkillButton(creature->skills[i], i);
            connect(button, &SkillButton::skillSelected, this, &SkillSelecter::skillSelected);
            skillSlots->addWidget(button, i / 2, i % 2);
        }
    }
}