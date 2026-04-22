#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <QObject>
#include <QString>
#include "main_page.h"
#include "schedule_manager.h"

class App_Controller : public QObject
{
    Q_OBJECT
public:
    App_Controller();
    ~App_Controller();

public slots:
    void handle_login_successful(const QString &username);

private:
    main_page *app_main_page;
    Schedule_Manager schedule_manager;   // ★ 멤버로 소유
};

#endif