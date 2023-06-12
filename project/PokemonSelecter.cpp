/************************************************************
 * \author 蔣博元
 ***********************************************************/
#include "PokemonSelecter.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

// PokemonButton

PokemonButton::PokemonButton(Creature* pokemon, int index, QWidget *parent)
    : QFrame(parent), pokemonPtr(pokemon), index(index), pokemonHp(nullptr)
{
    QHBoxLayout* gLayout = new QHBoxLayout(this);
    gLayout->setAlignment(Qt::AlignCenter);
    this->setFrameStyle(QFrame::Panel);
    this->setLineWidth(3);

    /* 圖片 */ {
        QLabel* img = new QLabel;
        img->setMinimumWidth(56);
        QPixmap imgPixmap(QString(":/media/Pokemon-pix/") + pokemon->getName().c_str());
        img->setPixmap(imgPixmap.scaled(56, 42, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        gLayout->addWidget(img);
    }

    // types
    QHBoxLayout* typeBox = new QHBoxLayout;
    for (int i = 0; i < pokemon->types.size(); ++i) {
        QLabel* typeImg = new QLabel;
        QPixmap imgPixmap(QString(":/media/types/") + pokemon->types[i].c_str());
        typeImg->setPixmap(imgPixmap.scaled(35, 35));
        typeBox->addWidget(typeImg);
    }
    typeBox->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    // name
    QLabel* pokemonName = new QLabel(pokemon->getName().c_str());
    pokemonName->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QVBoxLayout* typeAndName = new QVBoxLayout;
    typeAndName->addLayout(typeBox);
    typeAndName->addWidget(pokemonName);
    gLayout->addLayout(typeAndName);

    // hp
    pokemonHp = new QLabel;
    pokemonHp->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel* maxHp = new QLabel(QString("/") + QString::number(pokemon->getMaxHp()));
    gLayout->addWidget(pokemonHp);
    gLayout->addWidget(maxHp);
}

void PokemonButton::updateHp() {
    pokemonHp->setText(QString::number(pokemonPtr->getHp()));
    // 若hp歸零，設成disabled
    this->setDisabled(pokemonPtr->getHp() <= 0);
}

void PokemonButton::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton && pokemonPtr->getHp() > 0) {
        emit pokemonSelected(this);
    }
}

// PokemonSelecter

PokemonSelecter::PokemonSelecter(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* gLayout = new QHBoxLayout(this);
    gLayout->setContentsMargins(0, 0, 0, 0);

    // scroll area for pokemon
    {
        QWidget* w = new QWidget;
        pokemonSlots = new QGridLayout(w);
        QScrollArea* pokemonScroll = new QScrollArea;
        pokemonScroll->setWidget(w);
        pokemonScroll->setWidgetResizable(true);
        pokemonScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        gLayout->addWidget(pokemonScroll, 5);
    }

    // button back
    {
        buttonBack = new QPushButton("Back");
        buttonBack->setIcon(QIcon(":/media/back.png"));
        buttonBack->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        connect(buttonBack, &QPushButton::clicked, this, [this]() { emit pokemonSelected(nullptr); });
        gLayout->addWidget(buttonBack,1);
    }

    // palette
    {
        QPalette p = this->palette();
        p.setColor(QPalette::Active, QPalette::WindowText, QColor(30, 10, 82));
        p.setColor(QPalette::Active, QPalette::Dark, QColor(30, 10, 82));
        p.setColor(QPalette::Light, QColor(212, 202, 237));
        setPalette(p);
    }
}

void PokemonSelecter::init(Player *player) {
    // delete original pokemon button
    {
        QList<PokemonButton*> list = findChildren<PokemonButton*>();
        for (auto p : list) {
            pokemonSlots->removeWidget(p);
            delete p;
        }
    }

    // add new pokemons
    {
        std::vector<Creature*>& creatures = player->creatures;
        for (int i = 0; i < creatures.size(); ++i) {
            PokemonButton* button = new PokemonButton(creatures[i], i);
            connect(button, &PokemonButton::pokemonSelected, this, &PokemonSelecter::pokemonSelected);
            pokemonSlots->addWidget(button, i / 3, i % 3);
        }
    }
}
