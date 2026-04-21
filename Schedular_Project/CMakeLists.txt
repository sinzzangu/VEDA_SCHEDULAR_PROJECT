cmake_minimum_required(VERSION 3.19)
project(Schedular_Project LANGUAGES CXX)

set(CMAKE_AUTOUIC ON) #자동생성 되지않아 수동 추가함
set(CMAKE_AUTOMOC ON) #자동생성 되지않아 수동 추가함
set(CMAKE_AUTORCC ON) #자동생성 되지않아 수동 추가함

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Schedular_Project
    WIN32 MACOSX_BUNDLE
    main.cpp
    login_page.cpp
    login_page.h
    login_page.ui
    resources/resources.qrc
    login_register.h login_register.cpp login_register.ui


    login_error.h login_error.cpp login_error.ui


)

target_link_libraries(Schedular_Project
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS Schedular_Project
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Schedular_Project
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
