/***********************
 * 04/20 MON
 * schedule_dialog.h
 * 일정 상세 보기 및 편집 다이얼로그
 * Created By 방준한
 * Version 4.0
 * 스케쥴 변경 함수 만들기
 * 으헝헝 프로젝트 콤보박스로 변경............ 근데 선택은 불가능하게...?
 ***********************
 */
/***********************
 * 04/20 MON
 * schedule_dialog.h
 * 일정 상세 보기 및 편집 다이얼로그
 * Created By 방준한
 * Version 3.1
 * 프로젝트 콤보박스 지원 추가
 ***********************
 */
#ifndef SCHEDULE_DIALOG_H
#define SCHEDULE_DIALOG_H
#include <QDialog>
#include <QList>
#include "schedule.h"
#include "project.h"
namespace Ui {
class Schedule_Dialog;
}
class Schedule_Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Schedule_Dialog(Schedule &schedule, QList<Project> &projects, QWidget *parent = nullptr);
    // 일정 생성 관련 생성자
    ~Schedule_Dialog();
    explicit Schedule_Dialog(QList<Project> &projects, QString &default_project_name, QWidget *parent = nullptr);

    // 수정된 스케줄 가져오기
    Schedule get_updated_schedule();
    bool is_deleted();
    QString get_selected_project_name();


private slots:
    void handle_save_clicked();
    void handle_delete_clicked();
    void handle_dates_changed();
private:
    Ui::Schedule_Dialog *ui;
    Schedule s_schedule;
    QList<Project> s_projects;  // 프로젝트 목록 저장
    void populate_fields();
    void populate_project_combobox();  // 콤보박스 채우는 함수
    void update_progress_bar();
    int calculate_progress();
    // 생성자에서 바꾸기 귀찮음..
    bool schedule_was_deleted = false;

    // 일정 생성 관련
    bool s_is_new_mode;
    void setup_new_mode_defaults(QString &default_project_name);
};
#endif