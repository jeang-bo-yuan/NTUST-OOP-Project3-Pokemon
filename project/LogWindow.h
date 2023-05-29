
#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>
#include <QPlainTextEdit>
#include <string>
#include <sstream>


/**
 * @brief 顯示log的widget，繼承自QPlainTextEdit
 * @details 內含兩個class，可用來將ostream的輸出重新導向到LogWindow中
 */
class LogWindow : public QPlainTextEdit

{
    Q_OBJECT

public:
    /**
     * @brief stringbuf class for LogWindow
     * @details 建立一個輸出用buffer，可以用std::ostream的rdbuf方法來使用建好的buffer，flush時buffer內容會附加到LogWindow中（用textCursor->insertText）
     */
    class LogStringBuf : public std::stringbuf {
        LogWindow* logW; //!< where output goes
    protected:
        // Intend: 覆寫flush時做的事
        // Pre: none
        // Post: 將buffer中的文字輸出到LogWindow
        int sync() override;
    public:
        // Intend: 建構子
        // Pre: p指向LogWindow，LogWindow不能比LogStringBuf物件還要早解構
        // Post: 建立一個buffer，關聯到p
        LogStringBuf(LogWindow* p) : std::stringbuf(std::ios_base::out), logW(p) {}
    };

    /**
     * @brief stringbuf class for LogWindow, but for output error
     * @details 建立一個輸出用buffer，可以用std::ostream的rdbuf方法來使用建好的buffer，flush時buffer內容會附加到LogWindow中（用textCursor()->insertHtml）
     */
    class ErrorLogStringBuf : public std::stringbuf {
        LogWindow* logW; //!< where output goes
    protected:
        // Intend: 覆寫flush時做的事
        // Pre: none
        // Post: 將buffer內容以紅色文字輸出
        int sync() override;
    public:
        // Intend: 建構子
        // Pre: p指向LogWindow，LogWindow不能比ErrorLogStringBuf物件還要早解構
        // Post: 建立一個buffer，關聯到p
       ErrorLogStringBuf(LogWindow* p) : std::stringbuf(std::ios_base::out), logW(p) {}
    };

    // Intend: 建構子
    // Pre: none
    // Post: 設成唯讀
    LogWindow(QWidget *parent = nullptr);
    // Intend: 解構子
    // Pre: none
    // Post: none
    ~LogWindow();
};

#endif // LOGWINDOW_H
