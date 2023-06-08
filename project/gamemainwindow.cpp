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
    this->setMinimumSize(1000, 630);

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
    if (!ui->checkBoxCmdFile->isChecked()) {
        std::cout << "載入寶可夢... " << qPrintable(ui->filePokemon->getFile()) << std::endl;
        std::cout << "載入招式... " << qPrintable(ui->fileMove->getFile()) << std::endl;
        std::cout << "載入玩家... " << qPrintable(ui->fileGame->getFile()) << std::endl;

        std::fstream Fin(ui->filePokemon->getFile().toStdString());
        gameManager.currentPlayer->reset();
        while (true) {
            Creature* p = new Creature;
            Fin >> *p;
            if (p->getName() == "") break;
            gameManager.currentPlayer->creatures.push_back(p);
        }
        Fin.close();
        Fin.clear();
        Fin.open(ui->fileMove->getFile().toStdString());
        for (int i = 0; i < 6; ++i) {
            skill theSkill;
            Fin >> theSkill;
            gameManager.currentPlayer->creatures[0]->skills.emplace_back(std::move(theSkill));
        }
        for (auto p : gameManager.currentPlayer->objects) {
            p->setUsageCount(3);
        }
        std::cerr << "Warning: Game Data isn't loaded from game manager\n";
    }
    else {
        std::cout << "載入command file... " << qPrintable(ui->fileCmdFile->getFile()) << std::endl;
        std::cerr << "Loading command file isn't implemented yet\n";
        return;
    }

    // 切換stack
    ui->mainStack->setCurrentIndex(1);
    selectLogWindow();
    ui->optionGroup->show();

    // initialize selecters
    ui->subBagSelecter->init(gameManager.currentPlayer);
    ui->subPokemonSelecter->init(gameManager.currentPlayer);
    ui->subSkillSelecter->init(gameManager.currentPlayer->creatures[0]);
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
    std::cout << "use skill " << button->getIndex() << std::endl;
    std::cerr << "Warning: using skill isn't implemented in game manager yet!\n";

    selectLogWindow();
}

void GameMainWindow::pokemonSelected(PokemonButton *button)
{
    // let connection be destroyed after a pokemon is choosen
    disconnect(ui->subPokemonSelecter, &PokemonSelecter::pokemonSelected, this, &GameMainWindow::pokemonSelected);

    if (button != nullptr) {
        std::cout << button->getIndex() << " selected" << std::endl;
        std::cerr << "Warning: switching pokemon is implemented yet!\n";
        selectLogWindow();
    }
}


