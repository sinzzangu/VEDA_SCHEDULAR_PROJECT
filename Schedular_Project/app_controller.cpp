#include "app_controller.h"

App_Controller::App_Controller() : app_main_page(nullptr) {}

App_Controller::~App_Controller()
{
    delete app_main_page;
}

void App_Controller::handle_login_successful()
{
    app_main_page = new main_page();
    app_main_page->show();
}
