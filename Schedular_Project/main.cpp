#include "login_page.h"
#include "app_controller.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

    Widget login;
    App_Controller controller;

    QObject::connect(&login, SIGNAL(login_successful()),
                     &controller, SLOT(handle_login_successful()));

    login.show();
    return a.exec();

}