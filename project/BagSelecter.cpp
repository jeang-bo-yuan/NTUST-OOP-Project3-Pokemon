
#include "BagSelecter.h"
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollBar>

// ItemButton 的實作

// ctor
ItemButton::ItemButton(const std::string &_name, int _count, QWidget *parent)
    : QFrame(parent)
    , itemImage(QString(":/media/bag/") + _name.c_str()), itemName(new QLabel(_name.c_str()))
    , count(_count), itemCount(new QLabel(QString::number(count)))
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QLabel* img = new QLabel;
    img->setPixmap(QPixmap::fromImage(itemImage));

    hLayout->addWidget(img);
    hLayout->addWidget(itemName);
    hLayout->addWidget(itemCount);

    this->setMinimumHeight(70);
}

// deal with mouse clicked
void ItemButton::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton /* && count > 0 */) {
        --count;
        itemCount->setText(QString::number(count));
        emit itemSelected(itemName->text().toStdString());
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
    for (int i = 0; i < player->objects.size(); ++i) {
        Object* o = player->objects[i];
        ItemButton* button = new ItemButton(o->getName(), o->getUsageCount());

        bagSlots->addWidget(button, i / 3, i % 3);
        connect(button, &ItemButton::itemSelected, this, &BagSelecter::itemSelected);
    }
}
