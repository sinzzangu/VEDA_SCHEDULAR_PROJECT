cmake_minimum_required(VERSION 3.19)
project(Schedular_Project LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Schedular_Project
    WIN32 MACOSX_BUNDLE
    main.cpp

    main_page.h main_page.cpp main_page.ui
    main_page.h main_page.cpp main_page.ui
    schedule.h schedule.cpp
    project.h project.cpp
    schedule_bar.h schedule_bar.cpp
    date_header.h date_header.cpp

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
