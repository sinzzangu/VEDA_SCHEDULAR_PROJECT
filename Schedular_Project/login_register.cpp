#include "login_register.h"
#include "login_error.h"
#include "ui_login_register.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QCryptographicHash>
#include <QString>
#include <QDebug>

login_register::login_register(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login_register)
{
    ui->setupUi(this);
    //이메일 형식
    QRegularExpression email_regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    // 이메일 유효성 검사
    QRegularExpressionValidator *validator
        = new QRegularExpressionValidator(email_regex, this);
    // 이메일입력창에 적용
    ui->lineEdit_email->setValidator(validator);

}
// 비밀번호 SHA-256알고리즘으로 해싱
QString login_register::change_pw_hesh(const QString &password)
{
    QByteArray data = password.toUtf8();
    QByteArray hashed_pw = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    return hashed_pw.toHex();
}

// 회원가입시 사용자 데이터 Jason 파일 저장
void login_register::save_user_account()
{
    password = ui->lineEdit_pw->text();
    //Json 파일저장
    QFile file("user_data.json");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<< "에러";
        return;
    }
    QJsonObject userInfo;
    userInfo.insert("username", ui->lineEdit_username->text());
    userInfo.insert("email", ui->lineEdit_email->text());
    userInfo.insert("password", change_pw_hesh(password));
    userInfo.insert("createAt", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    all_user_data.insert(ui->lineEdit_username->text(), userInfo);

    //Json 문서생성
    QJsonDocument json_doc;
    json_doc.setObject(all_user_data);
    file.write(json_doc.toJson());
    file.close();
    qDebug() << "데이터 저장 완료: " << all_user_data.size() << "명";
}

//회원가입창 확인버튼 클릭시 동작
void login_register::on_pushButton_confirm_clicked()
{
    QRegularExpression email_regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    QRegularExpressionMatch match = email_regex.match(ui->lineEdit_email->text());

    //회원가입창 빈칸 오류
    if(ui->lineEdit_email->text()==""||ui->lineEdit_pw->text()==""||ui->lineEdit_username->text()=="")
    {
        login_error *null_data = new login_error(this);
        null_data->login_dialog_change_text(6);
        null_data->exec();
    }
    else if(!match.hasMatch())
    {
        login_error *missmatch_email = new login_error(this);
        missmatch_email->login_dialog_change_text(5);
        missmatch_email->exec();
    }
    else
    {
        //중복된 사용자명 알림
        if(all_user_data.contains(ui->lineEdit_username->text()))
        {
            login_register::save_user_account();
            login_error *duplicate_username = new login_error(this);
            duplicate_username->login_dialog_change_text(4);
            duplicate_username->exec();
        }
        else
        {
            //회원가입 성공, 비밀번호 불일치 알림
            if(ui->lineEdit_pw->text() == ui->lineEdit_pw_check->text()) // password, password check 동일여부 판단
            {
                login_register::save_user_account();
                login_error *success_page = new login_error(this);
                success_page->login_dialog_change_text(3);
                success_page->exec();

                accept();
            }
            else
            {
                login_register::save_user_account();
                login_error *mismatch_page = new login_error(this);
                mismatch_page->login_dialog_change_text(2);
                mismatch_page->exec();
            }
        }
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
