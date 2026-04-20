#ifndef LOGIN_REGISTER_H
#define LOGIN_REGISTER_H

#include <QDialog>
#include <QJsonDocument>
#include <QFile>

namespace Ui {
class login_register;
}

class login_register : public QDialog
{
    Q_OBJECT

public:
    explicit login_register(QWidget *parent = nullptr);
    ~login_register();


private slots:
    void on_pushButton_confirm_clicked();
    void on_pushButton_cancel_clicked();


private:
    Ui::login_register *ui;

    QString id_fix;
    QString pw_fix;

    int pw_check_check();
    void save_user_account(QString id, QString pw);
};

#endif // LOGIN_REGISTER_H
