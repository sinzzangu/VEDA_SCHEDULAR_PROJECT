/***********************
 * 04/20 MON
 * date_header].h
 * date_header class for timeline
 * 프로젝트마다 헤더(날짜) 기간 잡아서 위에 보여주기.
 * Created By 방준한
 * Version 2.0
 * 프로젝트 이름 받아서 왼쪽 상단에 배치
 ***********************
 */
#include "date_header.h"
#include <QLabel>
#include <QHBoxLayout>

Date_Header::Date_Header(QString &project_name,
                         QDate &view_start,
                         int view_days,
                         QWidget *parent)
    : QWidget(parent),
    s_project_name(project_name),
    s_view_start(view_start),
    s_view_days(view_days)
{
    setFixedHeight(44);

    s_layout = new QHBoxLayout(this);
    s_layout->setContentsMargins(0, 0, 0, 0);
    s_layout->setSpacing(0);

    build_cells();
}

void Date_Header::build_cells()
{
    // 1. 왼쪽 프로젝트 이름 라벨
    s_project_label = new QLabel(s_project_name, this);   // ⭐ 텍스트 = 프로젝트명
    s_project_label->setFixedWidth(200);
    s_project_label->setAlignment(Qt::AlignCenter);
    s_project_label->setStyleSheet(
        "background-color: white; "
        "font-size: 13px; "
        "font-weight: 500; "
        "color: #2E4A6B; "                        // ⭐ 사이드바와 같은 네이비
        "border-right: 1px solid #E5E8EB; "
        "border-bottom: 1px solid #E5E8EB;"
        );
    s_layout->addWidget(s_project_label);

    // 2. 각 날짜마다 셀 (기존과 동일)
    QDate today = QDate::currentDate();
    QStringList day_names = {"월", "화", "수", "목", "금", "토", "일"};

    for (int i = 0; i < s_view_days; i++) {
        QDate current_date = s_view_start.addDays(i);

        int day_of_week = current_date.dayOfWeek();
        bool is_weekend = (day_of_week == 6 || day_of_week == 7);
        bool is_today = (current_date == today);

        QString text = QString("%1\n%2")
                           .arg(day_names[day_of_week - 1])
                           .arg(current_date.day());

        QLabel *cell = new QLabel(text, this);
        cell->setAlignment(Qt::AlignCenter);

        QString style =
            "background-color: white; "
            "font-size: 10px; "
            "color: #8A8F95; "
            "border-right: 1px solid #F0F2F5; "
            "border-bottom: 1px solid #E5E8EB;";

        if (is_weekend) {
            style.replace("background-color: white;",
                          "background-color: #F5F7FA;");
        }

        if (is_today) {
            style =
                "background-color: #FFF5F5; "
                "font-size: 10px; "
                "font-weight: bold; "
                "color: #E24B4A; "
                "border-right: 1px solid #F0F2F5; "
                "border-bottom: 2px solid #E24B4A;";
        }

        cell->setStyleSheet(style);
        s_layout->addWidget(cell);
    }
}

// ⭐ 프로젝트 이름 업데이트용 함수 (나중에 콤보박스 연동할 때 쓸 것)
void Date_Header::set_project_name(QString &name)
{
    s_project_name = name;
    if (s_project_label) {
        s_project_label->setText(name);
    }
}