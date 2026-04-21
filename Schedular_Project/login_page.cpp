#include "login_page.h"
#include "login_register.h"
#include "ui_login_page.h"
#include "login_error.h"
#include <QDialog>
#include <QFrame>
#include <QPainter>
#include <QJsonObject>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFile file("user_data.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"user_data.json 열기 실패";
        return;
    }

    QString data = file.readAll();
    file.close();
    loadData(data);
}

Widget::~Widget()
{
    delete ui;
}
//기존 가입자 데이터 obj_allUserData public변수에 저장
void Widget::loadData(const QString &data)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    obj_allUserData = jsonResponse.object();
}
//로그인 데이터 일치여부 확인
int Widget::login_check()
{
    if(obj_allUserData.contains(id))
    {
        login_register heshpw;
        QJsonObject userData = obj_allUserData.value(id).toObject();
        QString userData_value = userData.value("password").toString();
        if(userData_value != heshpw.change_pwHesh(pw))
        {
            // qDebug() << "비번틀림";
            return 1;
        }
        // qDebug() << "로그인 성공";
        return 2;
    }
    // qDebug() << "아이디 없음";
    return 0;
}


void Widget::on_pushButton_register_clicked()
{
    login_register *register_page = new login_register(this);
    //회원가입 창에 데이터 보내기
    register_page->setAlluserData(obj_allUserData);

    if (register_page->exec() == QDialog::Accepted) {
        // 회원가입 창에서 업데이트된 데이터를 다시 받아옴
        obj_allUserData = register_page->getAlluserData();
    }
}


void Widget::on_pushButton_login_clicked()
{
    id=ui->lineEdit_id->text();
    pw=ui->lineEdit_pw->text();
    int check_value = login_check();

    //로그인 실패창 팝업
    if(check_value <= 1)
    {
        login_error *error_page = new login_error(this);
        error_page->login_dialog_changeText(check_value); // 로그인 실패 창 텍스트 변경
        error_page->exec();
    }
    else if(check_value == 2)
    {
        qDebug() << "메인창과 연동";
    }
    return;
}

