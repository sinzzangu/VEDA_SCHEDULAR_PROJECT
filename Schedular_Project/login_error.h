/***********************
 * 04/20 MON
 * login_error.cpp
 * Created By 강유근
 * Version 2.0
 * 로그인/회원가입시 나타나는 다이얼로그
 * 각 오류 번호마다 라벨 텍스트변경
 ***********************/

#ifndef LOGIN_ERROR_H
#define LOGIN_ERROR_H

#include <QDialog>

namespace Ui {
class login_error;
}

class login_error : public QDialog
{
    Q_OBJECT

public:
    explicit login_error(QWidget *parent = nullptr);
    ~login_error();

    void login_dialog_change_text(int check_value);

private slots:
    void on_pushButton_confirm_clicked();

private:
    Ui::login_error *ui;
};

#endif // LOGIN_ERROR_H
