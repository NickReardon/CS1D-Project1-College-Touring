#include "tripprogress.h"
#include "ui_tripprogress.h"

tripProgress::tripProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tripProgress)
{
    ui->setupUi(this);
}

tripProgress::~tripProgress()
{
    delete ui;
}
