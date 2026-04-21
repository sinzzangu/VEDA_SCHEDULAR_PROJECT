/***********************projec
 * 04/20 MON
 * main_page.h header file
 * Created By 방준한
 * Version 5.0
 * create cells for gantt chart view
 * create date header
 * activate combo box
 ***********************
 */
#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMainWindow>
#include "project.h"
#include <QVBoxLayout>
#include "date_header.h"
#include <QDateTime>
#include "schedule_dialog.h"
#include "project_dialog.h"


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
    // 개발 시 디비 파일 패스
    const QString schedular_json_path = "C:\\QtSource\\QT_Project\\Schedular_Project\\db\\schedules.json";

    // layouts
    QVBoxLayout *timeline_layout; // 타임라웃 적용할 레이아웃

    // member vars
    QList<Project> projects;
    QDate view_start_date; // 화면에 표시할 시작일
    QDate view_end_date; // 화면에 표시할 마지막 일
    int view_days; // 표시할 일 수
    Date_Header *date_header; // 헤더 (나중에 바뀔 수 있음)

    // functions
    void load_projects();
    void create_combo_box();
    void render_selected_project(Project &project);
    void render_all_projects();
    void clear_timeline();
    void rebuild_timeline();
    void recalculate_view_dates(Project &project);
    void calculate_all_dates();

    // schedule 처리 함수
    void update_schedule_in_projects(Schedule &updated);
    void delete_schedule_from_projects(QString id);
    void save_projects_to_json();

    // 일정 추가
    QString generate_schedule_id();
    void add_schedule_to_project(QString &project_name, Schedule &new_schedule);

    // 프로젝트 추가
    QString generate_project_id();
    bool is_project_name_duplicate(QString &name);
    Project_Dialog *s_active_project_dialog = nullptr;

// slots
private slots:
    void handle_project_changed(int index); // slot function for combo box
    void handle_schedule_clicked(Schedule schedule); // slot function for changning schedule
    // void handle_schedule_deleted();
    void handle_add_schedule_button_clicked(); // 일정 추가 버튼
    void handle_add_project_button_clicked(); // 프로젝트 추가 버튼

    void handle_project_add_requested(QString name);
    void handle_project_delete_requested(QString project_id);

};
#endif // MAIN_PAGE_H
