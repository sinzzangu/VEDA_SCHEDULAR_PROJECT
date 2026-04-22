#include "login_error.h"
#include "ui_login_error.h"

login_error::login_error(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login_error)
{
    ui->setupUi(this);
}

login_error::~login_error()
{
    delete ui;
}
//알림창 텍스트변경
void login_error::login_dialog_change_text(int check_value)
{
    if(check_value ==0)
    {
        ui->label_contents->setText("아이디가 존재하지 않습니다.");
        ui->label_error_type->setText("아이디 오류");
    }
    else if(check_value == 1)
    {
        ui->label_contents->setText("비밀번호가 틀렸습니다.");
        ui->label_error_type->setText("비밀번호 오류");
    }
    else if(check_value==2)
    {
        ui->label_contents->setText("비밀번호가 일치하지 않습니다.");
        ui->label_error_type->setText("비밀번호 불일치.");
    }
    else if(check_value==3)
    {
        ui->label_contents->setText("회원가입이 정상처리 되었습니다.");
        ui->label_error_type->setText("회원가입 완료.");
    }
    else if(check_value==4)
    {
        ui->label_contents->setText("이미 존재하는 사용자 이름입니다.");
        ui->label_error_type->setText("사용자 이름 중복.");
    }
    else if(check_value==5)
    {
        ui->label_contents->setText("잘못된 이메일 형식입니다.");
        ui->label_error_type->setText("이메일 형식오류");
    }
    else if(check_value==6)
    {
        ui->label_contents->setText("누락된 정보가 확인되었습니다.");
        ui->label_error_type->setText("정보 누락");
    }
}

void login_error::on_pushButton_confirm_clicked()
{
    reject();
}

