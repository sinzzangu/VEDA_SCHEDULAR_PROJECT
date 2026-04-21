#ifndef LOGIN_REGISTER_H
#define LOGIN_REGISTER_H

#include <QDialog>
#include <QFile>
#include <QDateTime>
#include <QJsonObject>

namespace Ui {
class login_register;
}

class login_register : public QDialog
{
    Q_OBJECT

public:
    explicit login_register(QWidget *parent = nullptr);
    ~login_register();

    QJsonObject getAlluserData() {return allUserData;}
    void setAlluserData(QJsonObject obj_allUserData) {allUserData=obj_allUserData;}
    QString change_pwHesh(const QString &password);

private slots:
    void on_pushButton_confirm_clicked();
    void on_pushButton_cancel_clicked();



private:
    Ui::login_register *ui;

    QJsonObject allUserData;
    QString password;


    void save_user_account();
};

#endif // LOGIN_REGISTER_H
