/***********************
 * 04/20 MON
 * project.h header file
 * project data holder class
 * Created By 방준한
 * Version 1.0
 ***********************
 */
#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QList>
#include "schedule.h"

class Project
{
public:
    Project();
    Project(QString &id, QString &name);

    // Getter
    QString get_id();
    QString get_name();
    QList<Schedule> get_schedules();

    // Setter
    void set_id(QString &id);
    void set_name(QString &name);

    // 일정 관련 메소드
    void add_schedule(Schedule &schedule); // 스케쥴 추가
    void remove_schedule_by_id(QString &scheduleId); // 스케쥴 취소
    int get_schedule_count(); // 스케쥴 갯수 리턴

private:
    QString project_id;
    QString project_name;
    QList<Schedule> project_schedules;
};

#endif