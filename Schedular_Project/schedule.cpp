/***********************
 * 04/20 MON
 * schedule.cpp
 * schedule data holder class
 * Created By 방준한
 * Version 2.0
 * 수정사항: schedule이 자기가 어떤 프로젝트에 속해있는지 알기 위해서 프로젝트 멤버 변수 추가
 ***********************
 */
#include "schedule.h"

Schedule::Schedule() {}

Schedule::Schedule(const QString &id,
             const QString &title,
             const QString &description,
             const QDate &start_date,
             const QDate &end_date,
             const QString &category,
                   int progress, QString &project_name): schedule_id(id), schedule_title(title), schedule_description(description), schedule_start_date(start_date), schedule_end_date(end_date), schedule_category(category), schedule_progress(progress), schedule_project_name(project_name){
}

// Getters 값 가져오기
QString Schedule::get_id() {
    return schedule_id;
}

QString Schedule::get_title(){
    return schedule_title;
}

QString Schedule::get_description(){
    return schedule_description;
}

QDate Schedule::get_start_date(){
    return schedule_start_date;
}

QDate Schedule::get_end_date(){
    return schedule_end_date;
}

QString Schedule::get_category(){
    return schedule_category;
}

int  Schedule::get_progress(){
    return schedule_progress;
}

QString Schedule::get_schedule_project_name(){
    return schedule_project_name;
}

// Setters 값 설정하기
void Schedule::set_id(QString &id){
    schedule_id = id;
}

void Schedule::set_title(QString &title){
    schedule_title = title;
}

void Schedule::set_description(QString &description){
    schedule_description = description;
}

void Schedule::set_start_date(QDate &date){
    schedule_start_date = date;
}

void Schedule::set_end_date(QDate &date){
    schedule_end_date = date;
}

void Schedule::set_category(QString &category){
    schedule_category = category;
}

void Schedule::set_progress(int progress){
    schedule_progress = progress;
}

void Schedule::set_schedule_project_name(QString &project_name) {
    schedule_project_name = project_name;
}
