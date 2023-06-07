/************************************************************
 * \author 蔣博元
 ***********************************************************/
#include "LogWindow.h"
#include <regex>
#include <QPalette>


LogWindow::LogWindow(QWidget *parent)
    : QTextEdit(parent)
{
    this->setReadOnly(true);
    this->setLineWrapMode(LineWrapMode::NoWrap);
}

LogWindow::~LogWindow()
{
}

int LogWindow::LogStringBuf::sync()
{
    std::string str = this->str();
    if (str.size() == 0) return 0;

    if (str.back() == '\n') str.pop_back();

    logW->moveCursor(QTextCursor::End);
    logW->setCurrentCharFormat(QTextCharFormat());  // clear format
    // append buffer contents
    if (!logW->textCursor().atStart()) logW->textCursor().insertText("\n");
    logW->textCursor().insertText(str.c_str());
    logW->moveCursor(QTextCursor::End);

    // clear buffer
    this->str("");
    return 0;
}

int LogWindow::ErrorLogStringBuf::sync()
{
    std::string str = this->str();
    if (str.size() == 0 || str.back() != '\n') return 0;

    str.pop_back();
    str = std::regex_replace(str, std::regex("\n"), "<br>");  // replace \n with <br>

    logW->moveCursor(QTextCursor::End);
    // append buffer contents
    logW->textCursor().insertHtml(QString("<span style=\"color:red;\"><br>") + str.c_str() + "</span>");
    logW->moveCursor(QTextCursor::End);

    // clear buffer
    this->str("");
    return 0;
}
