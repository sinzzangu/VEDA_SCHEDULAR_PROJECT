/***********************
 * 04/20 MON
 * schedule_bar.h header file
 * schedule_bar generator class
 * Created By 방준한
 * Version 2.0
 * 수정사항: 여러개의 label 만들고, 오늘 날짜에 해당하는 label들으 색깔에 변화주기.
 ***********************
 */
#ifndef SCHEDULE_BAR_H
#define SCHEDULE_BAR_H

#include <QWidget>
#include <QDate>
#include <QList>
#include "schedule.h"

class QLabel;
class QHBoxLayout;

class Schedule_Bar : public QWidget
{
    Q_OBJECT
public:
    Schedule_Bar(const Schedule &schedule,
                 const QDate &view_start,
                 int view_days,
                 QWidget *parent = nullptr);

private:
    void build_cells();   // 셀들을 생성하는 함수

    Schedule s_bar_schedule;
    QDate s_bar_view_start;
    int s_bar_view_days;

    QHBoxLayout *s_bar_layout;
    QLabel *s_bar_title_label;         // 제목
    QList<QLabel*> s_bar_date_cells;   // 날짜 셀들
};

#endif