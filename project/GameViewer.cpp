
#include "GameViewer.h"
#include <QTimer>
#include <QEventLoop>
#include "EffectManager.h"

// definition of static member
QSoundEffect GameViewer::damageSound;
QSoundEffect GameViewer::healSound;

void GameViewer::setData(Player *player)
{
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

    setCondition(player);

    for (int i = 0; i < player->creaturesSize(); ++i) {
        if (player->getCreature(i).getHp() == 0) {
            pokeBallBox->itemAt(i)->widget()->setDisabled(true);
        }
    }
}

void GameViewer::setCondition(Player *player)
{
    // todo
    QString cond = QString::fromStdString(EffectManager::getEffectStr(&player->getCurrentCreature()));
    // 有新的condition
    if (cond != condition->text()) {
        condition->setText(cond);

        condition->show();
        waitFor(100);
        condition->hide();
        waitFor(100);
        condition->show();
    }
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

        condition = new QLabel;

        QVBoxLayout* layout = new QVBoxLayout(w);
        QHBoxLayout* hL = new QHBoxLayout();
        hL->addWidget(name);
        hL->addWidget(condition);
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
    waitFor(50);

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
    // 讓圖片漸漸縮小的動畫
    QEventLoop loop;
    QTimer timer;
    connect(&timer, &QTimer::timeout, &loop, [&, this]() {
        static int count = 30;
        QPixmap p = pokemonImg->pixmap(Qt::ReturnByValue);
        pokemonImg->setPixmap(p.scaled(p.size() / 2));
        pokemonImg->repaint();
        if (count--) {
            loop.exit();
            timer.stop();
        }
    });
    timer.start(10);
    loop.exec();

    // 清空圖片
    pokemonImg->setPixmap(QPixmap());
    waitFor(100);

    condition->setText(QString());
    setData(player);
}

void GameViewer::useSkillAnimation(const string &skillName)
{
    if (skillName == "Splash") {
        QSoundEffect sound;
        sound.setSource(QUrl("qrc:/media/music/splash.wav"));
        QRect old = pokemonImg->geometry();
        QRect jump(old.x(), old.y() + 20, old.width(), old.height());

        for (int time = 2; time --; ) {
            sound.play();
            pokemonImg->setGeometry(jump);
            waitFor(100);
            pokemonImg->setGeometry(old);
            waitFor(100);
        }
    }
}

