/***********************************************************************
 * 04/20 MON
 * project.cpp
 *
 * [역할]
 *   하나의 프로젝트(업무 묶음)를 표현하는 도메인 객체.
 *   프로젝트 식별 정보(id, name)와 그에 속한 스케줄 목록을 함께 소유함.
 *
 * [주요 기능]
 *   - 프로젝트 id, 이름에 대한 getter/setter
 *   - 자신이 소유한 스케줄 리스트(project_schedules) 관리
 *     · add_schedule: 스케줄 추가
 *     · remove_schedule_by_id: id로 찾아 제거 (뒤에서부터 순회)
 *     · replace_schedule_by_id: id로 찾아 새 스케줄로 교체
 *   - get_schedule_count: 스케줄 개수 반환 (UI 표시용)
 *
 * Created By 방준한
 * Version 2.0
 ***********************************************************************/
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
    // 기존이랑 새로운거랑 id는 같으니까 그걸 기반으로 찾아서 수정
    for (int i = 0; i < project_schedules.size(); i++) {
        if (project_schedules[i].get_id() == schedule_id) {
            project_schedules[i] = new_schedule;
            return;   // 찾았으니 끝
        }
    }
}