/***********************
 * 04/20 MON
 * main_page.h header file
 * Created By 방준한
 * Version 6.0
 * 리팩토링: Schedule_Manager 분리
 ***********************
 */
#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H
#include <QMainWindow>
#include <QVBoxLayout>
#include "project.h"
#include "schedule.h"
#include "date_header.h"
#include "schedule_dialog.h"
#include "project_dialog.h"
#include "schedule_manager.h"

namespace Ui {
class main_page;
}

class main_page : public QMainWindow
{
    Q_OBJECT
public:
    explicit main_page(QWidget *parent = nullptr);
    ~main_page();

private:
    Ui::main_page *ui;

    // 데이터 매니저
    Schedule_Manager *schedule_manager;

    // 간트 차트 관련 변수들
    QVBoxLayout *timeline_layout;
    QDate view_start_date;
    QDate view_end_date;
    int view_days;
    Date_Header *date_header;

    // 간트 차트 함수들
    void create_combo_box();
    void render_selected_project(Project &project);
    void render_all_projects();
    void clear_timeline();
    void rebuild_timeline();
    void recalculate_view_dates(Project &project);
    void calculate_all_dates();

    // Project_Dialog 활성 포인터
    // 프로젝트 추가시 즉각 업데이트 하기 위해서.
    Project_Dialog *s_active_project_dialog = nullptr;

private slots:
    void handle_project_changed(int index);
    void handle_schedule_clicked(Schedule schedule);
    void handle_add_schedule_button_clicked();
    void handle_add_project_button_clicked();
    void handle_project_add_requested(QString name);
    void handle_project_delete_requested(QString project_id);
    void on_logout_button_clicked();
};
#endif // MAIN_PAGE_H