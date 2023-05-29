
#include "gamemainwindow.h"

#include <QApplication>

#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gameWindow->show();

    std::cout << u8"遊戲啟動完成" << std::endl;

    return a.exec();
}
