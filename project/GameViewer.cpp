
#include "GameViewer.h"
#include <QTimer>
#include <QEventLoop>

// definition of static member
QSoundEffect GameViewer::damageSound;
QSoundEffect GameViewer::healSound;

void GameViewer::setData(Player *player)
{
    setCondition(player);

    // image
    {
        QPixmap img (QString(":/media/Pokemon/") + player->getCurrentCreature().getName().c_str());
        pokemonImg->setPixmap(img.scaled(pokemonImg->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // name and hp
    {
        name->setText(player->getCurrentCreature().getName().c_str());
        hpBar->setRange(0, player->getCurrentCreature().getMaxHp());
        hpBar->setValue(player->getCurrentCreature().getHp());
    }

    for (int i = 0; i < player->creaturesSize(); ++i) {
        if (player->getCreature(i).getHp() == 0) {
            pokeBallBox->itemAt(i)->widget()->setDisabled(true);
        }
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
    damageSound.setSource(QUrl("qrc:/media/music/damage.wav"));
    healSound.setSource(QUrl("qrc:/media/music/heal.wav"));

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
        for (int i = 0; i < player->creaturesSize(); ++i) {
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
    int newHp = player->getCurrentCreature().getHp();
    setCondition(player);
    if (hpBar->value() == newHp) return;

    // 音效
    int delta = hpBar->value() > newHp ? -1 : 1;
    QSoundEffect& sound = delta == -1 ? damageSound : healSound;

    // 晃動
    QRect old = pokemonImg->geometry();
    pokemonImg->setGeometry(old.x() + 10, old.y(), pokemonImg->width(), pokemonImg->height());
    pokemonImg->repaint();

    // 以動畫顯示hp變化
    QEventLoop loop;
    QTimer timer;
    connect(&timer, &QTimer::timeout, &loop, [this, &loop, newHp, delta, old]() {
        pokemonImg->setGeometry(old);
        hpBar->setValue(hpBar->value() + delta);
        if (hpBar->value() == newHp) {
            loop.exit();
        }
    });
    timer.setInterval(10);

    sound.play();
    timer.start();
    loop.exec();
}

void GameViewer::switchPokemon(Player *player)
{
    setData(player);
}

