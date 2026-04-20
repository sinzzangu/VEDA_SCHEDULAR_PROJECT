/***********************projec
 * 04/20 MON
 * draw_gantt_row.cpp
 * 간트 차트 그리는 로직 클래스
 * Created By 방준한
 * Version 1.0
 ***********************
 */
#include "draw_gantt_row.h"

Draw_Gantt_Row::Draw_Gantt_Row(Schedule &schedule,
                               QDate &view_start,
                               QDate &view_end,
                               int view_days,
                               QWidget *parent):gantt_schedule(schedule), gantt_view_start(view_start), gantt_view_end(view_end), gantt_view_days(view_days), QWidget(parent){
    row_height;
}
