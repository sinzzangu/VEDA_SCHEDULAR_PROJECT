#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <QObject>
#include "main_page.h"
#include "login_page.h"

class App_Controller : public QObject
{
    Q_OBJECT
public:
    App_Controller();
    ~App_Controller();
    void set_login_window(Widget *login);  // 추가

public slots:
    void handle_login_successful();
    void handle_logout_requested();  // 추가

private:
    main_page *app_main_page;
    Widget *login_window;  // 추가
};

#endif