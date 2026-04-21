/***********************
 * 04/20 MON
 * schedule_bar.cpp
 * schedule_bar generator class
 * Created By 방준한
 * Version 5.0
 * 수정사항: 여러개의 label 만들고, 오늘 날짜에 해당하는 label들으 색깔에 변화주기.
 * clickable 만들고, 누르면 스케쥴 수정창 만들기
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
    setCursor(Qt::PointingHandCursor);

    // qDebug() << "Schedule_Bar 생성됨:" << s_bar_schedule.get_title();
}

void Schedule_Bar::build_cells()
{
    // 1. 프로젝트 타이틀 레이블 만들기
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

    // ⭐ 이 일정의 카테고리 기반 색상 미리 계산
    QString category = s_bar_schedule.get_category();
    QString main_color = get_category_main_color(category);
    QString border_color = get_category_border_color(category);

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
            // ⭐ 하드코딩 제거, 카테고리별 색 사용
            style = "background-color: " + main_color + "; "
                    + "border-right: 1px solid " + border_color + ";";
        }

        if (is_today && !is_in_range) {
            style += "background-color: #FFE8E8;";   // 연한 핑크
        }

        if (is_today) {
            style += "border-left: 2px solid #E24B4A;"
                     "border-right: 2px solid #E24B4A;";
        }
        cell->setStyleSheet(style);

        s_bar_layout->addWidget(cell);   // 남은 공간을 균등 분할
        // s_bar_date_cells.append(cell);
    }
}

void Schedule_Bar::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);   // 부모 기본 동작 호출

    // 시그널 발생!
    emit schedule_clicked(s_bar_schedule);

    qDebug() << "Schedule_Bar 클릭됨:" << s_bar_schedule.get_title();
}

// 색깔 정하기
QString Schedule_Bar::get_category_main_color(QString category)
{
    if (category == "work") {
        return "#5B8DD6";   // 블루
    }
    if (category == "study") {
        return "#6BB89A";   // 틸
    }
    if (category == "personal") {
        return "#D69A6B";   // 코랄
    }
    // fallback (event, 기타)
    return "#8A8F95";       // 회색
}

// ⭐ 카테고리별 테두리 색상 반환 (메인보다 살짝 어두움)
QString Schedule_Bar::get_category_border_color(QString category)
{
    if (category == "work") {
        return "#4A7AB8";
    }
    if (category == "study") {
        return "#569B82";
    }
    if (category == "personal") {
        return "#B88459";
    }
    // fallback
    return "#6E7378";
}