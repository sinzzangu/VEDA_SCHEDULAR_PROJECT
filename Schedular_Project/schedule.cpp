/***********************************************************************
 * 04/20 MON
 * schedule.cpp
 *
 * [역할]
 *   하나의 일정(스케줄)을 표현하는 도메인 객체.
 *   스케줄의 기본 속성(제목, 설명, 시작/종료일, 카테고리, 진행률)과
 *   자신이 소속된 프로젝트 이름을 함께 보관함.
 *
 * [주요 기능]
 *   - id, title, description, start_date, end_date, category, progress,
 *     project_name에 대한 getter / setter 제공
 *   - 모든 필드에 대한 초기값을 받는 생성자와 기본 생성자 지원
 *   - Qt의 시그널을 통해 객체가 전달될 수 있도록 복사 가능한 형태로 설계
 *     (main.cpp의 qRegisterMetaType<Schedule> 등록과 짝을 이룸)
 *
 * Created By 방준한
 * Version 2.0 (소속 프로젝트 이름 필드 추가)
 ***********************************************************************/
#include "schedule.h"

Schedule::Schedule()
{
}

Schedule::Schedule(const QString &id, const QString &title, const QString &description,
                   const QDate &start_date, const QDate &end_date, const QString &category,
                   int progress, QString &project_name)
    : schedule_id(id)
    , schedule_title(title)
    , schedule_description(description)
    , schedule_start_date(start_date)
    , schedule_end_date(end_date)
    , schedule_category(category)
    , schedule_progress(progress)
    , schedule_project_name(project_name)
{
}

// Getters 값 가져오기
QString Schedule::get_id()
{
    return schedule_id;
}

QString Schedule::get_title()
{
    return schedule_title;
}

QString Schedule::get_description()
{
    return schedule_description;
}

QDate Schedule::get_start_date()
{
    return schedule_start_date;
}

QDate Schedule::get_end_date()
{
    return schedule_end_date;
}

QString Schedule::get_category()
{
    return schedule_category;
}

int Schedule::get_progress()
{
    return schedule_progress;
}

QString Schedule::get_schedule_project_name()
{
    return schedule_project_name;
}

// Setters 값 설정하기
void Schedule::set_id(QString &id)
{
    schedule_id = id;
}

void Schedule::set_title(QString &title)
{
    schedule_title = title;
}

void Schedule::set_description(QString &description)
{
    schedule_description = description;
}

void Schedule::set_start_date(QDate &date)
{
    schedule_start_date = date;
}

void Schedule::set_end_date(QDate &date)
{
    schedule_end_date = date;
}

void Schedule::set_category(QString &category)
{
    schedule_category = category;
}

void Schedule::set_progress(int progress)
{
    schedule_progress = progress;
}

void Schedule::set_schedule_project_name(QString &project_name)
{
    schedule_project_name = project_name;
}
