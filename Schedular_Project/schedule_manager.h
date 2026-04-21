/***********************
 * 04/21 MON
 * schedule_manager.h
 * 스케줄/프로젝트 데이터 관리 + JSON 영속성
 * Created By 방준한
 * Version 1.0
 ***********************
 */
#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include "project.h"
#include "schedule.h"

class Schedule_Manager : public QObject
{
    Q_OBJECT
public:
    explicit Schedule_Manager(QObject *parent = nullptr);

    // 데이터 로딩/저장
    void load_projects();
    void save_projects_to_json();

    // projects 접근
    QList<Project> &get_projects();
    int get_project_count();

    // 스케줄 CRUD
    void add_schedule_to_project(QString &project_name, Schedule &new_schedule);
    void update_schedule_in_projects(Schedule &schedule);
    void delete_schedule_from_projects(QString id);

    // 프로젝트 CRUD
    void add_project(Project &new_project);
    void delete_project_by_id(QString project_id);

    // id 생성
    QString generate_schedule_id();
    QString generate_project_id();

    // 유틸
    bool is_project_name_duplicate(QString &name);

private:
    // 개발 시 디비 파일 패스
    const QString schedular_json_path = "C:\\QtSource\\QT_Project\\Schedular_Project\\db\\schedules.json";

    QList<Project> s_projects;
};

#endif // SCHEDULE_MANAGER_H