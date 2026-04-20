/***********************
 * 04/20 MON
 * schedule.h header file
 * schedule data holder class
 * Created By 방준한
 * Version 2.0
 * 수정사항: schedule이 자기가 어떤 프로젝트에 속해있는지 알기 위해서 프로젝트 멤버 변수 추가
 ***********************
 */
#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <QString>
#include <QDate>

class Schedule
{
public:
    Schedule();
    Schedule(const QString &id,
             const QString &title,
             const QString &description,
             const QDate &start_date,
             const QDate &end_date,
             const QString &category,
             int progress,
             QString &project_name);

    // Getters 값 가져오기
    QString get_id();
    QString get_title();
    QString get_description();
    QDate get_start_date();
    QDate get_end_date();
    QString get_category();
    QString get_schedule_project_name();
    int get_progress();

    // Setters 값 설정하기
    void set_id(QString &id);
    void set_title(QString &title);
    void set_description(QString &description);
    void set_start_date(QDate &date);
    void set_end_date(QDate &date);
    void set_category(QString &category);
    void set_progress(int progress);
    void set_schedule_project_name(QString &project_name);

private:
    // 멤버 변수들
    QString schedule_id;
    QString schedule_title;
    QString schedule_description;
    QDate schedule_start_date;
    QDate schedule_end_date;
    QString schedule_category;
    int schedule_progress;
    QString schedule_project_name;
};

#endif // SCHEDULE_H
