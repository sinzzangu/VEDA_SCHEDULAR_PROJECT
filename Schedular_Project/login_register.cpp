#include "login_register.h"
#include "login_register_mismatch.h"
#include "login_register_success.h"
#include "ui_login_register.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

login_register::login_register(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login_register)
{
    ui->setupUi(this);

}



void login_register::save_user_account(QString id, QString pw) // 회원가입시 사용자 데이터 Jason 파일 저장
{
    //Json 객체생성 및 데이터 삽입
    QJsonObject user_data;
    user_data.insert("id", id);
    user_data.insert("pw", pw);
    //Json 문서생성
    QJsonDocument json_doc(user_data);
    //Json 파일저장
    QFile file("user_data.json");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(json_doc.toJson());
        file.close();
        qDebug()<< "Json 저장 성공";
    }
    else
    {
        qDebug()<< "에러";
    }
}

int login_register::pw_check_check() //register 패스워드와 패스워드 확인 동일 여부 판단
{
    QString id= ui->lineEdit_id->text();;
    QString pw= ui->lineEdit_pw->text();;
    QString pw_check= ui->lineEdit_pw_check->text();
    id_fix = id;
    pw_fix = pw;

    qDebug() << "id" << id_fix;
    qDebug() << "pw" << pw_fix;
    // qDebug() << "pw_check" << pw;

    if(pw == pw_check) return 0;

    return -1;
}

void login_register::on_pushButton_confirm_clicked()
{

    // qDebug() << "checker mode :" <<pw_check_check();
    if(pw_check_check() == 0)
    {
        qDebug() << "id :" <<ui->lineEdit_id->text() <<"\npw :"<<ui->lineEdit_pw->text(); //임시 데이터 확인용 Json 으로 내보낼 예정
        login_register_success success_page(this);

        success_page.exec();
        login_register::save_user_account(id_fix, pw_fix);
        accept();
    }
    else
    {
        login_register_mismatch mismatch_page(this);
        mismatch_page.exec();
    }

}


void login_register::on_pushButton_cancel_clicked()
{
    reject();
}

login_register::~login_register()
{
    delete ui;
}
