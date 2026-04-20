/***********************
 * 04/20 MON
 * schedule_bar.cpp
 * schedule_bar generator class
 * Created By 방준한
 * Version 2.0
 * 수정사항: 여러개의 label 만들고, 오늘 날짜에 해당하는 label들으 색깔에 변화주기.
 ***********************
 */
#include "schedule_bar.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

Schedule_Bar::Schedule_Bar(const Schedule &schedule,
                           const QDate &view_start,
                           int view_days,
                           QWidget *parent)
    : QWidget(parent),
    s_bar_schedule(schedule),
    s_bar_view_start(view_start),
    s_bar_view_days(view_days)
{
    setFixedHeight(44);

    // 수평 레이아웃 생성
    s_bar_layout = new QHBoxLayout(this);
    s_bar_layout->setContentsMargins(0, 0, 0, 0);
    s_bar_layout->setSpacing(0);

    build_cells();

    // qDebug() << "Schedule_Bar 생성됨:" << s_bar_schedule.get_title();
}

void Schedule_Bar::build_cells()
{
    // 1. 제목 라벨 생성 (왼쪽 고정)
    QString title_text = s_bar_schedule.get_title();

    s_bar_title_label = new QLabel(title_text, this);
    s_bar_title_label->setFixedWidth(200);
    s_bar_title_label->setStyleSheet(
        "padding: 0 10px;"
        "font-size: 11px;"
        "color: #2C2F33;"
        "border-right: 1px solid #E5E8EB;"
        );
    s_bar_layout->addWidget(s_bar_title_label);

    // 2. 각 날짜마다 셀 하나씩 만들기
    QDate schedule_start = s_bar_schedule.get_start_date();
    QDate schedule_end = s_bar_schedule.get_end_date();
    QDate today = QDate::currentDate();

    for (int i = 0; i < s_bar_view_days; i++) {
        QDate current_date = s_bar_view_start.addDays(i);

        QLabel *cell = new QLabel("", this);

        // 이 날짜가 일정 범위 안에 있는지
        bool is_in_range = (current_date >= schedule_start
                            && current_date <= schedule_end);
        bool is_today = (current_date == today);
        bool is_weekend = (current_date.dayOfWeek() == 6
                           || current_date.dayOfWeek() == 7);

        // 스타일 결정
        QString style = "border-right: 1px solid #F0F2F5;";

        if (is_weekend) {
            style += "background-color: #F5F7FA;";
        }

        if (is_in_range) {
            style = "background-color: #5B8DD6; "   // 막대 색
                    "border-right: 1px solid #4A7AB8;";
        }

        if (is_today) {
            style += "border-left: 2px solid #E24B4A;";  // 오늘 빨간 선
        }

        cell->setStyleSheet(style);
        s_bar_layout->addWidget(cell);   // 남은 공간을 균등 분할
        s_bar_date_cells.append(cell);
    }
}