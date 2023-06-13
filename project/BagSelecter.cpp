/************************************************************
 * \author 蔣博元
 ***********************************************************/
#include "BagSelecter.h"
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollBar>

// ItemButton 的實作

// ctor
ItemButton::ItemButton(const Object& object, int index, QWidget* parent)
    : QFrame(parent)
    , index(index), count(object.getUsageCount()), itemCount(new QLabel(QString::number(count)))
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QLabel* itemName = new QLabel(object.getName().c_str());
    itemName->setMinimumWidth(100);
    itemName->setAlignment(Qt::AlignCenter);
    QLabel* img = new QLabel;
    img->setPixmap(QPixmap(QString(":/media/bag/") + object.getName().c_str()));

    hLayout->addWidget(img);
    hLayout->addWidget(itemName);
    hLayout->addWidget(itemCount);
    hLayout->setAlignment(Qt::AlignCenter);
    hLayout->setSpacing(10);

    this->setMinimumHeight(70);
    this->setDisabled(count <= 0);
}

void ItemButton::useOne() {
    --count;
    itemCount->setText(QString::number(count));
    this->setDisabled(count <= 0);
}

// deal with mouse clicked
void ItemButton::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton && count > 0) {
        emit itemSelected(this);
    }
}

// BagSelecter 的實作

// ctor
BagSelecter::BagSelecter(QWidget* parent)
    : QScrollArea(parent), bagSlots(nullptr)
{
    QWidget* w = new QWidget;
    bagSlots = new QGridLayout(w);
    this->setWidget(w);

    this->setWidgetResizable(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

// initialize item bag
void BagSelecter::init(const Player *player)
{
    // delete original ItemButton
    QList<ItemButton*> buttons = findChildren<ItemButton*>();
    for (auto p : buttons) {
        bagSlots->removeWidget(p);
        delete p;
    }

    // add ItemButton
    for (int i = 0; i < player->objectsSize(); ++i) {
        ItemButton* button = new ItemButton(player->getObject(i), i);

        bagSlots->addWidget(button, i / 3, i % 3);
        connect(button, &ItemButton::itemSelected, this, &BagSelecter::itemSelected);
    }
}
