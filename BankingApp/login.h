#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include "secondscreen.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    bool isInfoValid();
    void clearSenderLabels();
    void enableAfterLogin();
    void updateBalancesData();
    bool exists = false;
    bool receiverExists = false;

    ~Login();

private slots:
    void on_loginButton_clicked();

    void on_sendButton_clicked();

    void on_withdrawButton_clicked();

    void on_depositButton_clicked();

private:
    QSqlDatabase myDatabase;
    QString email;
    QString receiverEmail;
    QString receiverClientId;
    QString password;
    int balance;
    QString name;
    QString bankName;
    QString clientId;
    int withdrawAmount;
    int depositAmount;
    SecondScreen *send;
    QString receiverBankName;
    int amountSend;
    int receiverBalance;


    Ui::Login *ui;
};
#endif // LOGIN_H
