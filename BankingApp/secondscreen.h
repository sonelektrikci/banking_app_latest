#ifndef SECONDSCREEN_H
#define SECONDSCREEN_H

#include <QDialog>


namespace Ui {
class SecondScreen;
}

class SecondScreen : public QDialog
{
    Q_OBJECT

public:
    explicit SecondScreen(QWidget *parent = nullptr);
    ~SecondScreen();
    QString getReceiverEmailData();
    QString getReceiverIdData();
    int getAmountSendData();


private slots:
    void on_sendScreenButton_clicked();

private:
    //Login *login;
    Ui::SecondScreen *ui;
};

#endif // SECONDSCREEN_H
