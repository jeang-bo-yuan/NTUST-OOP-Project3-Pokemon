/**************************************************//**
 * \author 蔣博元
 * \date 2023/05/29
 *****************************************************/
#ifndef FILESELECTER_H
#define FILESELECTER_H


#include <QWidget>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>

/**
 * @brief 選擇檔案用，含輸入框+按鈕
 */
class FileSelecter : public QWidget
{
    Q_OBJECT
    QFileDialog dialog;
    QLineEdit* fileLine;
    QPushButton* buttonSelect;

public:
    explicit FileSelecter(QWidget *parent = nullptr);

    // 設置顯示的檔名
    void setFile(const QString& file);
    // 取得檔名
    QString getFile() const;

public slots:
    // 觸發按鈕
    void exec();

signals:

};

#endif // FILESELECTER_H
