
#include "gamemainwindow.h"
#include "ui_gamemainwindow.h"

#include "LogWindow.h"
#include <iostream>

GameMainWindow::GameMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameMainWindow)
{
    ui->setupUi(this);

    // IO重導向
    std::cout.rdbuf(new LogWindow::LogStringBuf(ui->logWindow));
    std::cerr.rdbuf(new LogWindow::ErrorLogStringBuf(ui->logWindow));

    // default looks
    ui->optionGroup->hide();
    this->setMinimumSize(1000, 630);

    // connect signal and slots
    connect(ui->buttonStartGame, &QPushButton::clicked, this, &GameMainWindow::startGame);
    connect(ui->buttonRun, &QPushButton::clicked, this, &GameMainWindow::runAway);
    connect(ui->buttonBattle, &QPushButton::clicked, this, &GameMainWindow::selectBattle);
    connect(ui->buttonPokemon, &QPushButton::clicked, this, &GameMainWindow::selectPokemon);
    connect(ui->buttonBag, &QPushButton::clicked, this, &GameMainWindow::selectBag);
}

GameMainWindow::~GameMainWindow()
{
    delete ui;
}

GameMainWindow* GameMainWindow::singleWindow = nullptr;

GameMainWindow* GameMainWindow::singleton() {
    return singleWindow == nullptr ? (singleWindow = new GameMainWindow) : singleWindow;
}



void GameMainWindow::startGame() {
    std::cout << "載入資料...\n";
    std::cerr << "Loading Game Data isn't implemented yet\n";

    // 切換stack
    ui->mainStack->setCurrentIndex(1);
    ui->subStack->setCurrentIndex(0);
    ui->optionGroup->show();

    // 取消選取
    ui->buttonBattle->setChecked(false);
    ui->buttonPokemon->setChecked(false);
    ui->buttonBag->setChecked(false);
}

void GameMainWindow::runAway() {
    std::cout << "玩家逃跑了" << std::endl;

    // 切換stack
    ui->mainStack->setCurrentIndex(0);
    ui->subStack->setCurrentIndex(0);
    ui->optionGroup->hide();
}

void GameMainWindow::selectBattle() {
    ui->buttonPokemon->setChecked(false);
    ui->buttonBag->setChecked(false);

    if (ui->subStack->currentIndex() == 1) {
        ui->buttonBattle->setChecked(false);
        // 切回log
        ui->subStack->setCurrentIndex(0);
    }
    else {
        ui->subStack->setCurrentIndex(1);
        // show buttons and let player choose
    }
}

void GameMainWindow::selectPokemon() {
    ui->buttonBattle->setChecked(false);
    ui->buttonBag->setChecked(false);

    if (ui->subStack->currentIndex() == 2) {
        ui->buttonPokemon->setChecked(false);
        // 切回log
        ui->subStack->setCurrentIndex(0);
    }
    else {
        ui->subStack->setCurrentIndex(2);
        // show buttons and let player choose
    }
}

void GameMainWindow::selectBag() {
    ui->buttonBattle->setChecked(false);
    ui->buttonPokemon->setChecked(false);

    if (ui->subStack->currentIndex() == 3) {
        ui->buttonBag->setChecked(false);
        // 切回log
        ui->subStack->setCurrentIndex(0);
    }
    else {
        ui->subStack->setCurrentIndex(3);
        // show buttons and let player choose
    }
}


