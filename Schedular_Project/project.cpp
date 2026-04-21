/***********************projec
 * 04/20 MON
 * project.h
 * 프로젝트 객체 홀더
 * Created By 방준한
 * Version 1.0
 ***********************
 */
#include "project.h"

Project::Project() {}

Project::Project(QString &id, QString &name): project_id(id), project_name(name) {}

// Getter
QString Project::get_id(){
    return project_id;
}

QString Project::get_name(){
    return project_name;
}

QList<Schedule> Project::get_schedules(){
    return project_schedules;
}

int Project::get_schedule_count(){
    return project_schedules.size();
}


// Setter
void Project::set_id(QString &id){
    project_id = id;
}

void Project::set_name(QString &name){
    project_name = name;
}

// 일정 관련 메소드
void Project::add_schedule(Schedule &schedule)
{
    project_schedules.append(schedule);
}

void Project::remove_schedule_by_id(QString &schedule_id){
    // id로 스케쥴 찾고, 있으면 지우기.
    for (int i = project_schedules.size() - 1; i >= 0; --i) {
        if (project_schedules[i].get_id() == schedule_id) {
            project_schedules.removeAt(i);
        }
    }
}

void Project::replace_schedule_by_id(QString &schedule_id, Schedule &new_schedule)
{
    // id로 스케줄 찾고, 있으면 새 스케줄로 교체
    for (int i = 0; i < project_schedules.size(); i++) {
        if (project_schedules[i].get_id() == schedule_id) {
            project_schedules[i] = new_schedule;
            return;   // 찾았으니 끝
        }
    }
}