
#include "FileSelecter.h"
#include <QHBoxLayout>
#include <QSizePolicy>

FileSelecter::FileSelecter(QWidget *parent)
    : QWidget{parent}, dialog(this, "Select File")
    , fileLine(new QLineEdit(this)), buttonSelect(new QPushButton("...", this))
{
    // only existing file can be chosen
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);

    // connect signal & slots
    connect(buttonSelect, &QPushButton::clicked, this, &FileSelecter::exec);
    connect(&dialog, &QFileDialog::fileSelected, fileLine, &QLineEdit::setText);

    // 設置
    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->addWidget(fileLine);
    hlayout->addWidget(buttonSelect);
    buttonSelect->setFixedWidth(25);
}

void FileSelecter::setFile(const QString &file)
{
    fileLine->setText(file);
}

QString FileSelecter::getFile() const
{
    return fileLine->text();
}

void FileSelecter::exec()
{
    dialog.exec();
}

