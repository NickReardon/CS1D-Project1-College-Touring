#include "confirmpage.h"
#include "ui_confirmpage.h"

confirmpage::confirmpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::confirmpage)
{
    ui->setupUi(this);
}

confirmpage::~confirmpage()
{
    delete ui;
}

bool confirmpage::getData()
{
    qDebug() << "getData()";
    return answer;
}

void confirmpage::on_okButton_released()
{
    answer = true;
    hide();
}

void confirmpage::on_cancelButton_released()
{
    answer = false;
    hide();
}
