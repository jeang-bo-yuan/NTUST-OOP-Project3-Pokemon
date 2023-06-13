/************************************************************
 * \author 蔣博元
 ***********************************************************/
#include "gamemainwindow.h"
#include "ui_gamemainwindow.h"

#include "LogWindow.h"
#include <iostream>
#include <fstream>

#include <QPainter>
#include <QImage>
#include <QPalette>
#include <QEventLoop>
#include <QDebug>

#define SUBSTACK_LOG 0
#define SUBSTACK_BATTLE 1
#define SUBSTACK_POKEMON 2
#define SUBSTACK_BAG 3

GameMainWindow::GameMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameMainWindow), backgroundImg(":/media/background.png")
{
    ui->setupUi(this);

    // IO重導向
    std::cout.rdbuf(new LogWindow::LogStringBuf(ui->logWindow));
    std::cerr.rdbuf(new LogWindow::ErrorLogStringBuf(ui->logWindow));

    // default looks
    ui->optionGroup->hide();
    ui->fileMove->setFile("MoveLib.txt");
    ui->filePokemon->setFile("PokemonLib.txt");
    ui->fileGame->setFile("GameData.txt");
    ui->fileCmdFile->setFile("case.txt");
    this->setMinimumSize(1121, 698);
    ui->playerView->layout()->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    ui->computerView->layout()->setAlignment(Qt::AlignTop | Qt::AlignRight);

    // 半透明背景色
    QPalette backgroundP;
    backgroundP.setColor(QPalette::Window, QColor(235, 213, 141, 150));
    ui->pageSetting->setPalette(backgroundP);

    // 開始畫面signal and slots
    connect(ui->buttonStartGame, &QPushButton::clicked, this, &GameMainWindow::startGame);
    connect(ui->buttonSetting, &QPushButton::clicked, ui->settingStack, [p=ui->settingStack](){p->setCurrentIndex(1);});
    connect(ui->buttonBackToMain, &QPushButton::clicked, ui->settingStack, [p=ui->settingStack](){p->setCurrentIndex(0);});
    connect(ui->checkBoxCmdFile, &QCheckBox::toggled, ui->fileStack, [this](bool b) {
        ui->fileStack->setCurrentIndex(static_cast<int>(b));
    });

    // 戰鬥畫面signale and slots
    connect(ui->buttonRun, &QPushButton::clicked, this, &GameMainWindow::runAway);
    connect(ui->buttonBattle, &QPushButton::clicked, this, &GameMainWindow::selectBattle);
    connect(ui->buttonPokemon, &QPushButton::clicked, this, &GameMainWindow::selectPokemon);
    connect(ui->buttonBag, &QPushButton::clicked, this, &GameMainWindow::selectBag);
    connect(ui->subBagSelecter, &BagSelecter::itemSelected, this, &GameMainWindow::itemSelected);
    connect(ui->subSkillSelecter, &SkillSelecter::skillSelected, this, &GameMainWindow::skillSelected);
    std::cerr << "Warning: Connection between Game and GameViewer isn't established\n";
}

GameMainWindow::~GameMainWindow()
{
    delete ui;
}

GameMainWindow* GameMainWindow::singleWindow = nullptr;

GameMainWindow* GameMainWindow::singleton() {
    return singleWindow == nullptr ? (singleWindow = new GameMainWindow) : singleWindow;
}

void GameMainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(this->rect(), backgroundImg);
}

int GameMainWindow::choosePokemon(bool allowNull) {
    ui->subPokemonSelecter->setAllowNull(allowNull);

    int originalSubStackIndex = ui->subStack->currentIndex();
    ui->subStack->setCurrentIndex(SUBSTACK_POKEMON);
    bool originalVisible = ui->optionGroup->isVisible();
    ui->optionGroup->hide();

    QEventLoop loop;
    connect(ui->subPokemonSelecter, &PokemonSelecter::pokemonSelected, &loop, [&loop](PokemonButton* button) {
        // exit with pokemon index
        if (button != nullptr)
            loop.exit(button->getIndex());
        else
            loop.exit(-1);
    });
    int result = loop.exec();

    ui->subStack->setCurrentIndex(originalSubStackIndex);
    ui->optionGroup->setVisible(originalVisible);
    return result;
}

void GameMainWindow::startGame() {
    ui->logWindow->clear();

    if (!ui->checkBoxCmdFile->isChecked()) {
        std::string pokemonLib = ui->filePokemon->getFile().toStdString();
        std::string moveLib = ui->fileMove->getFile().toStdString();
        std::string gameData = ui->fileGame->getFile().toStdString();

        std::cout << "載入寶可夢... " << pokemonLib << std::endl;
        std::cout << "載入招式... " << moveLib << std::endl;
        std::cout << "載入玩家... " << gameData << std::endl;
        gameManager.loadGame(pokemonLib, moveLib, gameData);
    }
    else {
        std::cout << "載入command file... " << qPrintable(ui->fileCmdFile->getFile()) << std::endl;
        std::cerr << "Loading command file isn't implemented yet\n";
        return;
    }

    // initialize selecters
    player = gameManager.getCurrentPlayer();
    computer = gameManager.getNotCurrentPlayer();
    ui->subBagSelecter->init(player);
    ui->subPokemonSelecter->init(player);
    ui->subSkillSelecter->init(&player->getCurrentCreature());
    ui->playerView->init(player);
    ui->computerView->init(computer);

    // 切換stack
    ui->mainStack->setCurrentIndex(1);
    selectLogWindow();
    ui->optionGroup->show();
}

// 按下optionGroup中的按鈕的slot... ////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameMainWindow::runAway() {
    std::cout << "玩家逃跑了" << std::endl;

    // 切換stack
    ui->mainStack->setCurrentIndex(0);
    selectLogWindow();
    ui->optionGroup->hide();
}

void GameMainWindow::selectBattle() {
    uncheckOptions();

    if (ui->subStack->currentIndex() == SUBSTACK_BATTLE) {
        // 切回log
        selectLogWindow();
    }
    else {
        ui->buttonBattle->setChecked(true);
        // show buttons and let player choose
        ui->subStack->setCurrentIndex(SUBSTACK_BATTLE);
    }
}

void GameMainWindow::selectPokemon() {
    uncheckOptions();

    if (ui->subStack->currentIndex() == SUBSTACK_POKEMON) {
        selectLogWindow();
    }
    else {
        ui->buttonPokemon->setChecked(true);
        // show buttons and let player choose
        ui->subPokemonSelecter->setAllowNull(false);
        ui->subStack->setCurrentIndex(SUBSTACK_POKEMON);
        // 這個連結是一次性的
        connect(ui->subPokemonSelecter, &PokemonSelecter::pokemonSelected, this, &GameMainWindow::pokemonSelected, Qt::UniqueConnection);

    }
}

void GameMainWindow::selectBag() {
    uncheckOptions();

    if (ui->subStack->currentIndex() == SUBSTACK_BAG) {
        // 切回log
        selectLogWindow();
    }
    else {
        ui->buttonBag->setChecked(true);
        // show buttons and let player choose
        ui->subStack->setCurrentIndex(SUBSTACK_BAG);
    }
}

void GameMainWindow::selectLogWindow() {
    uncheckOptions();

    ui->subStack->setCurrentIndex(SUBSTACK_LOG);
}

void GameMainWindow::uncheckOptions() {
    ui->buttonBattle->setChecked(false);
    ui->buttonPokemon->setChecked(false);
    ui->buttonBag->setChecked(false);
    ui->subPokemonSelecter->reject();
}

// 玩家選擇要幹嘛後的slots //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameMainWindow::itemSelected(ItemButton* button)
{
    int pokemonIndex = choosePokemon(true);

    if (pokemonIndex != -1) {
        std::cout << "Player use " << button->getIndex() << " on " << pokemonIndex << std::endl;
        std::cerr << "Warning: using item isn't implemented in game manager yet!\n";
        button->useOne();
        selectLogWindow();
    }
}

void GameMainWindow::skillSelected(SkillButton *button)
{
    selectLogWindow();
    ui->optionGroup->hide();

    int playerSpeed = player->getCurrentCreature().getSpeed();
    int computerSpeed = computer->getCurrentCreature().getSpeed();
    qDebug() << "(Speed) Player: " << playerSpeed << "Computer: " << computerSpeed;

    // 為了流程控制，塞了一個沒用的迴圈
    do {
        if (playerSpeed >= computerSpeed) {
            // 玩家攻擊
            gameManager.humanAttack(button->getIndex());
            // 更新對手血量
            ui->computerView->updateHp(computer);

            // 對手掛了，不能攻擊
            if (computer->getCurrentCreature().getHp() <= 0)
                break;

            waitFor(500);
            // 對手攻擊
            gameManager.computerAttack(rand() % computer->getCurrentCreature().getSkillSize());
            // 更新玩家血量
            ui->playerView->updateHp(player);
        }
        else {
            // 電腦攻擊
            gameManager.computerAttack(rand() % computer->getCurrentCreature().getSkillSize());
            // 更新玩家血量
            ui->playerView->updateHp(player);

            // 玩家掛了
            if (player->getCurrentCreature().getHp() <= 0)
                break;

            waitFor(500);
            // 玩家攻擊
            gameManager.humanAttack(button->getIndex());
            // 更新對手血量
            ui->computerView->updateHp(computer);
        }
    } while(0);

    nextRound();
}

void GameMainWindow::pokemonSelected(PokemonButton *button)
{
    // let connection be destroyed after a pokemon is choosen
    disconnect(ui->subPokemonSelecter, &PokemonSelecter::pokemonSelected, this, &GameMainWindow::pokemonSelected);

    int idx = button->getIndex();
    if (button != nullptr) {
        ui->optionGroup->hide();
        selectLogWindow();

        // 切換pokemon
        gameManager.swapCreature(idx);
        ui->playerView->switchPokemon(player);
        ui->subSkillSelecter->init(&player->getCurrentCreature());

        waitFor(500);
        // 電腦攻擊
        gameManager.computerAttack(rand() % computer->getCurrentCreature().getSkillSize());
        // 更新玩家血量
        ui->playerView->updateHp(player);

        nextRound();
    }
}



/////////////////////////////////////////////////////////////////

void GameMainWindow::nextRound()
{
    qDebug() << "(Hp) Player: " << player->getCurrentCreature().getHp() << "Computer: " << computer->getCurrentCreature().getHp();

    // 玩家掛了
    if (player->getCurrentCreature().getHp() <= 0) {
        if (player->isAlive()) {
            int pokemonIndex = choosePokemon();
            gameManager.swapCreature(pokemonIndex);
            ui->playerView->switchPokemon(player);
            ui->subSkillSelecter->init(&player->getCurrentCreature());
        }
        else {
            std::cout << "You Lose" << std::endl;
            ui->mainStack->setCurrentIndex(0);
            return;
        }
    }

    // 電腦掛了
    if (computer->getCurrentCreature().getHp() <= 0) {
        if (computer->isAlive()) {
            // 找到current creature的id，接著switch到i + 1
            for (int i = 0; i < computer->creaturesSize(); ++i) {
                if (&computer->getCreature(i) == &computer->getCurrentCreature()) {
                    computer->switchCurrentCreature(i + 1);
                    ui->computerView->switchPokemon(computer);
                    break;
                }
            }
        }
        else {
            std::cout << "You Win" << std::endl;
            ui->mainStack->setCurrentIndex(0);
            return;
        }
    }

    ui->optionGroup->show();
}


