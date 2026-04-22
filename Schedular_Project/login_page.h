/***********************
 * 04/20 MON
 * login_page.h
 * Created By 강유근
 * Version 2.0
 * 로그인 위젯
 * 로그인시 저장된 Jason 읽어 기존 사용자 판별
 * 로그인 성공시 메인페이지에 시그널 전달
 ***********************/
#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "ui_login_page.h"
#include <QWidget>
#include <QFrame>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void on_pushButton_register_clicked();

    void on_pushButton_login_clicked();


signals:
    // 로그인시 유저내네임 보내주기
    void login_successful(const QString &username);

private:
    Ui::Widget *ui;
    QJsonObject obj_all_user_data;

    QString id;
    QString pw;

    void load_data(const QString &data);
    int login_check();
};



#endif // LOGIN_PAGE_H


