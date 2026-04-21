/***********************projec
 * 04/20 MON
 * draw_gantt_row.h
 * 간트 차트 그리는 헤더
 * Created By 방준한
 * Version 1.0
 ***********************
 */
#ifndef GANTT_ROW_H
#define GANTT_ROW_H

#include <QWidget>
#include <QDate>
#include "schedule.h"

class Draw_Gantt_Row : public QWidget
{
    Q_OBJECT
public:
    Draw_Gantt_Row(Schedule &schedule,
            QDate &view_start, QDate &view_end,
             int view_days,
             QWidget *parent = nullptr);

protected:
    // gantt chart row 하나 그리는 함수
    void create_gantt_chart_row();

private:
    Schedule gantt_schedule;
    QDate gantt_view_start;
    QDate gantt_view_end;
    int gantt_view_days;

    int title_width = 140;
    int row_height = 44;
    int bar_height = 24;
};

#endif