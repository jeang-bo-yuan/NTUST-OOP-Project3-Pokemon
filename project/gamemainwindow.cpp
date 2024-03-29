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
#include <QMediaPlaylist>

#define SUBSTACK_LOG 0
#define SUBSTACK_BATTLE 1
#define SUBSTACK_POKEMON 2
#define SUBSTACK_BAG 3

GameMainWindow::GameMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameMainWindow), backgroundImg(":/media/background.png")
{
    ui->setupUi(this);

    // bgm
    QMediaPlaylist* list = new QMediaPlaylist;
    list->setPlaybackMode(QMediaPlaylist::Loop);
    list->addMedia(QUrl("qrc:/media/music/bgm.mp3"));
    bgm.setPlaylist(list);
    bgm.setVolume(40);

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
        gameManager.setTesting(ui->testingModeCheckBox->isChecked());
        std::string pokemonLib = ui->filePokemon->getFile().toStdString();
        std::string moveLib = ui->fileMove->getFile().toStdString();
        std::string gameData = ui->fileGame->getFile().toStdString();

        std::cout << "載入寶可夢... " << pokemonLib << std::endl;
        std::cout << "載入招式... " << moveLib << std::endl;
        std::cout << "載入玩家... " << gameData << std::endl;

        try {
            gameManager.loadGame(pokemonLib, moveLib, gameData);
        }
        catch (int) {
            std::cerr << "Game Loading failed\n";
            return;
        }
    }
    else {
        gameManager.setTesting(false);
        std::string cmdFile = ui->fileCmdFile->getFile().toStdString();

        std::cout << "載入command file... " << cmdFile << std::endl;
        try {
            int status = gameManager.loadFromFile(cmdFile);
            if (status == 1)
                return;
        }
        catch (int) {
            std::cerr << "Game Loading failed\n";
            return;
        }
    }

    bgm.setPosition(0);
    bgm.play();

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
    backToMain();
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
        ui->optionGroup->hide();
        selectLogWindow();
        qDebug() << "use object" << button->getIndex() << "on" << pokemonIndex;

        // use object
        std::cout << gameManager.useObject(&player->getObject(button->getIndex())
                                         , &player->getCreature(pokemonIndex));
        std::cout.flush();
        ui->playerView->updateHp(player);

        waitFor(500);
        // 對手攻擊
        ComputerAttack();

        nextRound();
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
            PlayerAttack(button->getIndex());

            // 對手掛了，不能攻擊
            if (computer->getCurrentCreature().getHp() <= 0)
                break;

            waitFor(500);
            // 對手攻擊
            ComputerAttack();
        }
        else {
            // 電腦攻擊
            ComputerAttack();

            // 玩家掛了
            if (player->getCurrentCreature().getHp() <= 0)
                break;

            waitFor(500);
            // 玩家攻擊
            PlayerAttack(button->getIndex());
        }
    } while(0);

    nextRound();
}

void GameMainWindow::pokemonSelected(PokemonButton *button)
{
    // let connection be destroyed after a pokemon is choosen
    disconnect(ui->subPokemonSelecter, &PokemonSelecter::pokemonSelected, this, &GameMainWindow::pokemonSelected);

    if (button != nullptr) {
        int idx = button->getIndex();
        ui->optionGroup->hide();
        selectLogWindow();

        // 切換pokemon
        gameManager.swapCreature(idx);
        ui->playerView->switchPokemon(player);
        ui->subSkillSelecter->init(&player->getCurrentCreature());

        waitFor(500);
        // 電腦攻擊
        ComputerAttack();

        nextRound();
    }
}

void GameMainWindow::PlayerAttack(int skillIndex)
{
    // 玩家攻擊
    gameManager.humanAttack(skillIndex);
    ui->playerView->useSkillAnimation(player->getCurrentCreature().getSkillName(skillIndex));
    // 更新對手血量
    ui->computerView->updateHp(computer);
}

void GameMainWindow::ComputerAttack()
{
    // 電腦攻擊
    int idx = rand() % computer->getCurrentCreature().getSkillSize();
    gameManager.computerAttack(idx);
    ui->computerView->useSkillAnimation(computer->getCurrentCreature().getSkillName(idx));
    // 更新玩家血量
    ui->playerView->updateHp(player);
}



/////////////////////////////////////////////////////////////////

void GameMainWindow::nextRound()
{
    qDebug() << "(Hp) Player: " << player->getCurrentCreature().getHp() << "Computer: " << computer->getCurrentCreature().getHp();

    gameManager.nextRound_BandP();
    waitFor(500);
    ui->playerView->updateHp(player);
    ui->computerView->updateHp(computer);

    if (gameManager.winOrLose()) {
        backToMain();
        return;
    }

    // 玩家掛了
    if (player->getCurrentCreature().getHp() <= 0) {
        int pokemonIndex = choosePokemon();
        gameManager.changeCreature(false, pokemonIndex);
        ui->playerView->switchPokemon(player);
        ui->subSkillSelecter->init(&player->getCurrentCreature());
    }

    // 電腦掛了
    if (computer->getCurrentCreature().getHp() <= 0) {
        // 找到current creature的id，接著switch到i + 1
        for (int i = 0; i < computer->creaturesSize(); ++i) {
            if (&computer->getCreature(i) == &computer->getCurrentCreature()) {
                gameManager.changeCreature(true, i + 1);
                ui->computerView->switchPokemon(computer);
                break;
            }
        }
    }

    ui->optionGroup->show();
    gameManager.nextTurn();
}

void GameMainWindow::backToMain()
{
    bgm.stop();
    ui->mainStack->setCurrentIndex(0);
    selectLogWindow();
    ui->optionGroup->hide();
}


