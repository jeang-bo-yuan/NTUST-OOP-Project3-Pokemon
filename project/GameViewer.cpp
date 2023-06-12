
#include "GameViewer.h"
#include <QTimer>
#include <QEventLoop>

void GameViewer::setData(Player *player)
{

    setCondition(player);

    // name and hp
    {
        name->setText(player->currentCreature->getName().c_str());
        hpBar->setRange(0, player->currentCreature->getMaxHp());
        hpBar->setValue(player->currentCreature->getHp());
    }

    // image
    {
        QPixmap img (QString(":/media/Pokemon/") + player->currentCreature->getName().c_str());
        pokemonImg->setPixmap(img.scaled(pokemonImg->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void GameViewer::setCondition(Player *player)
{
   // todo
}

/////////////////////////////////////////////////////////////////////////

GameViewer::GameViewer(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // poke ball
    {
        pokeBallBox = new QHBoxLayout;
        pokeBallBox->setAlignment(Qt::AlignLeft);
        mainLayout->addLayout(pokeBallBox);
    }

    // name and hp
    {
        QWidget* w = new QWidget;
        w->setMaximumWidth(300);
        w->setFont(QFont("Arial", 10));
        w->setAutoFillBackground(true);

        name = new QLabel;
        name->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
        hpBar = new QProgressBar;
        hpBar->setFormat("%v/%m");

        condition = new QHBoxLayout;

        QVBoxLayout* layout = new QVBoxLayout(w);
        QHBoxLayout* hL = new QHBoxLayout();
        hL->addWidget(name);
        hL->addLayout(condition);
        layout->addLayout(hL);
        layout->addWidget(hpBar);

        mainLayout->addWidget(w);
    }

    // image
    {
        pokemonImg = new QLabel;
        pokemonImg->setFixedSize(250, 250);
        mainLayout->addWidget(pokemonImg);
    }
}

void GameViewer::init(Player *player)
{
    // pokeBall
    {
        // delete all items
        QLayoutItem* l;
        while ((l = pokeBallBox->takeAt(0)) != nullptr) {
            delete l->widget();
            delete l;
        }

        // add
        for (int i = 0; i < player->creatures.size(); ++i) {
            QLabel* pokeBall = new QLabel;
            pokeBall->setPixmap(QPixmap(":/media/pokemon_ball.png").scaled(20, 20));
            pokeBall->setFixedSize(20, 20);
            pokeBallBox->addWidget(pokeBall);
        }
    }

    setData(player);
}

void GameViewer::updateHp(Player *player)
{
    int newHp = player->currentCreature->getHp();
    setCondition(player);
    if (hpBar->value() == newHp) return;

    // 以動畫顯示hp變化
    int delta = hpBar->value() > newHp ? -1 : 1;
    QEventLoop loop;
    QTimer timer;
    connect(&timer, &QTimer::timeout, &loop, [this, &loop, newHp, delta]() {
        hpBar->setValue(hpBar->value() + delta);
        if (hpBar->value() == newHp)
            loop.exit();
    });
    timer.start(10);
    loop.exec();
}

void GameViewer::switchPokemon(Player *player)
{
    setData(player);

    for (int i = 0; i < player->creatures.size(); ++i) {
        if (player->creatures[i]->getHp() == 0) {
            pokeBallBox->itemAt(i)->widget()->setDisabled(true);
        }
    }
}
