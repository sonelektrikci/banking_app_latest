#include "login.h"
#include "ui_login.h"
#include <QtSql>
#include <QtDebug>
#include <iostream>
#include <QMessageBox>
#include<complex>
#include<string>


Login::Login(QWidget *parent)
    : QMainWindow(parent)
    ,ui(new Ui::Login)
    ,send(new SecondScreen)
{
    ui->setupUi(this);
    myDatabase = QSqlDatabase::addDatabase("QSQLITE");
    myDatabase.setDatabaseName("/Users/fatiherdogan/Desktop/projects/Database/BankingApp.db");

    if(!myDatabase.open()){
        qDebug() << "Database is not opened";
    }
    else{
        qDebug() << "Database is open";

    }
}
Login::~Login()
{
    delete ui;
    delete send;
}

bool Login::isInfoValid(){
    email = ui->emailEdit->text();
    password = ui->passwordEdit->text();

    QSqlQuery checkQuery(myDatabase);
    checkQuery.prepare("Select * FROM Customers WHERE Email = (:email) AND Password = (:password)");
    checkQuery.bindValue(":email",email);
    checkQuery.bindValue(":password",password);

    if(checkQuery.exec()){
        if(checkQuery.next()){
            exists = true;
        }
        else {
            clearSenderLabels();
            QMessageBox::StandardButton reply;
              reply = QMessageBox::warning(this, "Warning!" , QString("Wrong email or password!"),
                                            QMessageBox::NoButton);
        }
    }
    return exists;
}

void Login::on_loginButton_clicked()
{
    if(isInfoValid()){
        QSqlQuery query(myDatabase);
        query.prepare("SELECT * FROM Customers WHERE Email ='"+email+"'");
        query.bindValue(":email",email);

        if(query.exec()){
            if(query.next()){
                balance = query.value(4).toInt();
                name = query.value(1).toString();
                clientId = query.value(0).toString();
                bankName = query.value(5).toString();

                ui->clientIdLabel->setText(QString("Client Id: %1").arg(clientId));
                ui->nameLabel->setText(QString("Name: %1").arg(name));
                ui->bankNameLabel->setText(QString("Bank Name: %1").arg(bankName));
                ui->balanceLabel->setText(QString("Balance: $%1").arg(balance));
                enableAfterLogin();
        }
    }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this,"No Customer Info!",QString("I could not get the customer info!"),QMessageBox::NoButton);
        }
}
}

void Login::clearSenderLabels(){
    ui->clientIdLabel->setText("Client Id: ");
    ui->nameLabel->setText("Name: ");
    ui->bankNameLabel->setText("Bank Name: ");
    ui->balanceLabel->setText("Balance: $");
}
void Login::enableAfterLogin(){
    ui->sendButton->setEnabled(true);
    ui->withdrawButton->setEnabled(true);
    ui->depositButton->setEnabled(true);
    ui->withdrawAmountEdit->setEnabled(true);
    ui->depositAmountEdit->setEnabled(true);
}



void Login::on_withdrawButton_clicked()
{
    withdrawAmount = ui->withdrawAmountEdit->text().toInt();
    QSqlQuery query(myDatabase);
    query.prepare("SELECT * FROM Customers WHERE Email ='"+email+"'");
    query.bindValue(":email",email);


    if(query.exec()){
        if(query.next()){
            if(withdrawAmount<balance){
                QMessageBox::StandardButton reply;
                  reply = QMessageBox::question(this, "Warning!" , QString("Are you sure want to withdraw $ %1 ?").arg(withdrawAmount),QMessageBox::Yes|QMessageBox::No);
                  if (reply == QMessageBox::Yes) {
                      balance = balance - withdrawAmount;
                      if(myDatabase.open()){
                          query.prepare("UPDATE Customers SET Balance= (:balance) WHERE Email=(:email)");
                          query.bindValue(":balance",balance);
                          query.bindValue(":email",email);
                          if(query.exec()){
                              qDebug()<<"Query worked";
                              ui->balanceLabel->setText(QString("Balance: $%1").arg(balance));
                              QMessageBox::StandardButton reply;
                              reply = QMessageBox::information(this,"Withdraw Successful!",QString("Your balance is $%1 now").arg(balance),QMessageBox::NoButton);
                              ui->withdrawAmountEdit->clear();
                          }
                      }
                  }
            }
            else {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::warning(this, "Warning!" , QString("Insufficient Balance."), QMessageBox::NoButton);
            }
        }
    }
}


void Login::on_depositButton_clicked()
{
    depositAmount = ui->depositAmountEdit->text().toInt();
    QSqlQuery query(myDatabase);
    query.prepare("SELECT * FROM Customers WHERE Email ='"+email+"'");
    query.bindValue(":email",email);


    if(query.exec()){
        if(query.next()){
            balance = balance + depositAmount;
            if(myDatabase.open()){
                query.prepare("UPDATE Customers SET Balance= (:balance) WHERE Email=(:email)");
                query.bindValue(":balance",balance);
                query.bindValue(":email",email);
                if(query.exec()){
                    qDebug()<<"Query worked";
                    ui->balanceLabel->setText(QString("Balance: $%1").arg(balance));
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::information(this,"Deposit Successful!",QString("Your balance is $%1 now").arg(balance),QMessageBox::NoButton);
                    ui->depositAmountEdit->clear();
                }
            }
        }
    }
}

void Login::on_sendButton_clicked()
{
    send->exec();
    updateBalancesData();

}


void Login::updateBalancesData(){

    receiverEmail = send->getReceiverEmailData();
    receiverClientId = send->getReceiverIdData();
    amountSend = send->getAmountSendData();

    QSqlQuery checkQuery(myDatabase);
    checkQuery.prepare("Select * FROM Customers WHERE Email = (:email) AND ClientId = (:clientId)");
    checkQuery.bindValue(":email",receiverEmail);
    checkQuery.bindValue(":clientId",receiverClientId);

    if(checkQuery.exec()){
        if(checkQuery.next()){
            qDebug()<<"Check Query Worked";
            receiverExists = true;
        }
        else {
            qDebug()<<"Check query did not work";
        }
    }
    if(receiverExists){
        QSqlQuery receiverQuery(myDatabase);
        receiverQuery.prepare("SELECT * FROM Customers WHERE Email =(:email)");
        receiverQuery.bindValue(":email",receiverEmail);

        if(receiverQuery.exec()){
            if(receiverQuery.next()){
                if(amountSend<balance){
                    receiverBalance = receiverQuery.value(4).toInt();
                    receiverBankName = receiverQuery.value(5).toString();

                    receiverBalance = receiverBalance + amountSend;
                    receiverQuery.prepare("UPDATE Customers SET Balance= (:balance) WHERE Email=(:email)");
                    receiverQuery.bindValue(":balance",receiverBalance);
                    receiverQuery.bindValue(":email",receiverEmail);
                    if(receiverQuery.exec()){
                        qDebug()<<QString("Receiver Balance: %1").arg(receiverBalance);
            }
        }
        else {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this, "Warning!" , QString("Insufficient Balance."), QMessageBox::NoButton);
        }
    }
    if(receiverExists){
            if(amountSend<balance){
                if(bankName == receiverBankName){
                    balance = balance - amountSend;
                    QSqlQuery senderQuery(myDatabase);
                    senderQuery.prepare("UPDATE Customers SET Balance= (:balance) WHERE Email=(:email)");
                    senderQuery.bindValue(":balance",balance);
                    senderQuery.bindValue(":email",email);


                    qDebug()<<QString("Sender Balance: %1").arg(balance);
                    if(senderQuery.exec()){
                        qDebug()<<"Sender Successful";
                        ui->balanceLabel->setText(QString("Balance: $%1").arg(balance));
                    }
                    else {
                        qDebug()<<"Sender unsuccessful";
                    }
                }
                else {
                    balance = balance - amountSend -1;
                    QSqlQuery senderQuery(myDatabase);
                    senderQuery.prepare("UPDATE Customers SET Balance= (:balance) WHERE Email=(:email)");
                    senderQuery.bindValue(":balance",balance);
                    senderQuery.bindValue(":email",email);


                    qDebug()<<QString("Sender Balance: %1").arg(balance);
                    if(senderQuery.exec()){
                        qDebug()<<"Sender Successful";
                        ui->balanceLabel->setText(QString("Balance: $%1").arg(balance));
                    }
                    else {
                        qDebug()<<"Sender unsuccessful";
                    }
                }
            }
            else {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::warning(this, "Warning!" , QString("Insufficient Balance."), QMessageBox::NoButton);
            }

    }

}
}
}
