
#include "LogWindow.h"
#include <regex>
#include <QPalette>


LogWindow::LogWindow(QWidget *parent)
    : QPlainTextEdit(parent)
{
    this->setReadOnly(true);
    this->setLineWrapMode(LineWrapMode::NoWrap);
}

LogWindow::~LogWindow()
{
}

int LogWindow::LogStringBuf::sync()
{
    logW->moveCursor(QTextCursor::End);
    logW->setCurrentCharFormat(QTextCharFormat());  // clear format

    // append buffer contents
    logW->textCursor().insertText(this->str().c_str());
    logW->moveCursor(QTextCursor::End);

    // clear buffer
    this->str("");
    return 0;
}

int LogWindow::ErrorLogStringBuf::sync()
{
    std::string str = this->str();
    str = std::regex_replace(str, std::regex("\n"), "<br>");  // replace \n with <br>

    logW->moveCursor(QTextCursor::End);
    // append buffer contents
    logW->textCursor().insertHtml(QString(R"(<span style="color:red;">)") + str.c_str() + "</span>");
    logW->moveCursor(QTextCursor::End);

    // clear buffer
    this->str("");
    return 0;
}
