/***********************************************************************
 * 04/22 MON
 * app_controller.cpp
 *
 * [역할]
 *   로그인, 메인 페이지 띄우는 컨트롤러
 *
 * [주요 기능]
 *   -
 *
 * Created By 방준한
 * Version 2.0
 ***********************************************************************/

#include "app_controller.h"

App_Controller::App_Controller()
    : app_main_page(nullptr)
{
}

App_Controller::~App_Controller()
{
    delete app_main_page;
}

void App_Controller::handle_login_successful(const QString &username)
{
    // 유저 데이터 로드
    schedule_manager.set_current_user(username);

    // main_page에 매니저 포인터 전달
    app_main_page = new main_page(&schedule_manager);
    app_main_page->show();
}