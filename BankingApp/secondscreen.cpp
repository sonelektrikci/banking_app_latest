#include "secondscreen.h"
#include "ui_secondscreen.h"


SecondScreen::SecondScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondScreen)
    //login(new Login)
{
    ui->setupUi(this);

}

SecondScreen::~SecondScreen()
{
    delete ui;
}

void SecondScreen::on_sendScreenButton_clicked()
{
    QWidget::close();
}


QString SecondScreen::getReceiverEmailData(){
    return ui->receiverEmailEdit->text();
}
QString SecondScreen::getReceiverIdData(){
    return ui->receiverIdEdit->text();
}
int SecondScreen::getAmountSendData(){
    return ui->amountSendEdit->text().toInt();
}
