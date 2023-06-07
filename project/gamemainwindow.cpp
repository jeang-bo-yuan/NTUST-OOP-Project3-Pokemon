
#include "gamemainwindow.h"
#include "ui_gamemainwindow.h"

#include "LogWindow.h"
#include <iostream>
#include <fstream>

#include <QPainter>
#include <QImage>
#include <QPalette>
#include <QEventLoop>

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
    ui->subStack->setCurrentIndex(2);
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
}

void GameMainWindow::runAway() {
    std::cout << "玩家逃跑了" << std::endl;

    // 切換stack
    ui->mainStack->setCurrentIndex(0);
    selectLogWindow();
    ui->optionGroup->hide();
}

void GameMainWindow::selectBattle() {
    uncheckOptions();

    if (ui->subStack->currentIndex() == 1) {
        // 切回log
        selectLogWindow();
    }
    else {
        ui->buttonBattle->setChecked(true);
        ui->subStack->setCurrentIndex(1);
        // show buttons and let player choose
    }
}

void GameMainWindow::selectPokemon() {
    int index = choosePokemon(true);
    if (index >= 0) {
        std::cout << index << " selected" << std::endl;
        std::cerr << "Warning: Pokemon index isn't send to game manager\n";
    }
    selectLogWindow();
}

void GameMainWindow::selectBag() {
    uncheckOptions();

    if (ui->subStack->currentIndex() == 3) {
        // 切回log
        selectLogWindow();
    }
    else {
        ui->buttonBag->setChecked(true);
        ui->subStack->setCurrentIndex(3);
        // show buttons and let player choose
    }
}

void GameMainWindow::selectLogWindow() {
    uncheckOptions();

    ui->subStack->setCurrentIndex(0);
}

void GameMainWindow::uncheckOptions() {
    ui->buttonBattle->setChecked(false);
    ui->buttonPokemon->setChecked(false);
    ui->buttonBag->setChecked(false);
}

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


