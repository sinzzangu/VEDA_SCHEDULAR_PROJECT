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

#include "project.h"
#include "schedule.h"
#include <QCoreApplication>
#include <QList>
#include <QObject>
#include <QString>

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

    // 사용자
    void set_current_user(const QString &username);
    QString get_current_username();

private:
    // 개발 시 디비 파일 패스
    QString schedular_json_path = "";
    QList<Project> s_projects;

    QString session_username;

    void create_default_schedule_file();

};

#endif // SCHEDULE_MANAGER_H