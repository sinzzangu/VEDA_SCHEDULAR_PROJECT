/***********************
 * 04/20 MON
 * login_register.cpp
 * Created By 강유근
 * Version 2.0
 * 회원가입 다이얼로그
 * 회원가입시 기존회원, 정보 기입 누락, 이메일 형식 오류시 다이얼로그 팝업
 * 저장되어있는 기존 Jason 데이터를 받아와 신규 회원가입자정보 누적 기입
 ***********************/
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

    QJsonObject get_all_user_data() {return all_user_data;}
    void set_all_user_data(QJsonObject obj_all_user_data) {all_user_data=obj_all_user_data;}
    QString change_pw_hesh(const QString &password);

private slots:
    void on_pushButton_confirm_clicked();
    void on_pushButton_cancel_clicked();



private:
    Ui::login_register *ui;

    QJsonObject all_user_data;
    QString password;


    void save_user_account();
};

#endif // LOGIN_REGISTER_H
