#include "app_controller.h"
#include "login_page.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    Widget login;
    App_Controller controller;

    // 로그인 시그널 연결.
    QObject::connect(&login, SIGNAL(login_successful(QString)), &controller,
                     SLOT(handle_login_successful(QString)));

    login.show();
    return a.exec();
}