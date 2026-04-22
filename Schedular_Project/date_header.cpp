/***********************************************************************
 * 04/20 MON
 * date_header.cpp
 *
 * [역할]
 *   간트 차트 상단의 날짜 헤더를 그리는 커스텀 위젯.
 *   왼쪽에는 프로젝트 이름, 오른쪽에는 표시 기간 동안의 날짜 눈금을
 *   3단(년 / 월 / 일+요일) 계층 구조로 배치함.
 *
 * [주요 기능]
 *   - 프로젝트 이름 라벨(좌측 200px 고정)과 날짜 영역(우측)을 분할
 *   - 년/월 행은 같은 년·월끼리 span으로 묶어 넓게 표시
 *   - 일 행은 요일(한글) + 날짜를 함께 표시
 *   - 주말(토/일) 배경 음영, 오늘 날짜 빨간색 강조
 *
 * Created By 방준한
 * Version 4.0 (3단 계층 헤더 + 프로젝트명 왼쪽 배치)
 ***********************************************************************/
#include "date_header.h"
#include <QHBoxLayout>
#include <QLabel>

Date_Header::Date_Header(QString &project_name, QDate &view_start, int view_days, QWidget *parent)
    : QWidget(parent)
    , s_project_name(project_name)
    , s_view_start(view_start)
    , s_view_days(view_days)
{
    // set widget height
    setFixedHeight(72);
    s_layout = new QHBoxLayout(this);
    s_layout->setContentsMargins(0, 0, 0, 0);
    s_layout->setSpacing(0);

    // 매우 중요, 날짜 받아와서 이제 칸 나눠서 ㅁ나들어주기.
    build_cells();
}

void Date_Header::build_cells()
{
    // 1. 왼쪽 프로젝트 이름 라벨
    s_project_label = new QLabel(s_project_name, this);
    s_project_label->setFixedWidth(200);
    s_project_label->setAlignment(Qt::AlignCenter);
    s_project_label->setStyleSheet("background-color: white; "
                                   "font-size: 13px; "
                                   "font-weight: 500; "
                                   "color: #2E4A6B; "
                                   "border-right: 1px solid #E5E8EB; "
                                   "border-bottom: 1px solid #E5E8EB;");
    s_layout->addWidget(s_project_label);

    // 2. 오른쪽 날짜 영역 (QVBoxLayout으로 3줄) 맨 위부터 년, 월, 일
    QWidget *date_area = new QWidget(this);
    QVBoxLayout *date_vlayout = new QVBoxLayout(date_area);
    date_vlayout->setContentsMargins(0, 0, 0, 0);
    date_vlayout->setSpacing(0);

    // 2-1. 년도 행 (QHBoxLayout)
    QHBoxLayout *year_layout = new QHBoxLayout();
    year_layout->setContentsMargins(0, 0, 0, 0);
    year_layout->setSpacing(0);
    build_year_row(year_layout); // 년도 label 만들어서 넣어주기
    date_vlayout->addLayout(year_layout);

    // 2-2. 월 행
    QHBoxLayout *month_layout = new QHBoxLayout();
    month_layout->setContentsMargins(0, 0, 0, 0);
    month_layout->setSpacing(0);
    build_month_row(month_layout); // 그 아래에 월 label들 만들어서 넣어주기
    date_vlayout->addLayout(month_layout);

    // 2-3. 일 행
    QHBoxLayout *day_layout = new QHBoxLayout();
    day_layout->setContentsMargins(0, 0, 0, 0);
    day_layout->setSpacing(0);
    build_day_row(day_layout); // 마지막에 일 label들 만들어서 넣어주기
    date_vlayout->addLayout(day_layout);

    s_layout->addWidget(date_area);
}

void Date_Header::build_year_row(QHBoxLayout *layout)
{
    // 년도별로 표시
    // 날짜 계산해서, 년도가 2개면 두개 표시
    // ex) 2026년 30일 + 2027년 5일 → 두 개의 라벨

    int start_idx = 0;
    int current_year = s_view_start.year();

    for (int i = 0; i <= s_view_days; i++)
    {
        // 마지막이거나 년도가 바뀌는 시점
        bool is_last = (i == s_view_days);
        bool year_changed = false;

        if (!is_last)
        {
            QDate d = s_view_start.addDays(i);
            year_changed = (d.year() != current_year);
        }

        if (is_last || year_changed)
        {
            // start_idx ~ i-1 까지가 같은 년도
            int span = i - start_idx; // 몇 일이 같은 년도인지

            QLabel *year_label = new QLabel(QString::number(current_year) + "년", this);
            year_label->setAlignment(Qt::AlignCenter);
            year_label->setStyleSheet("background-color: #FAFBFC; "
                                      "font-size: 11px; "
                                      "font-weight: 500; "
                                      "color: #2C2F33; "
                                      "border-right: 1px solid #E5E8EB; "
                                      "border-bottom: 1px solid #E5E8EB;");

            // span 넣어서 stretch 해 주기
            layout->addWidget(year_label, span);

            if (year_changed)
            {
                start_idx = i;
                current_year = s_view_start.addDays(i).year();
            }
        }
    }
}

void Date_Header::build_month_row(QHBoxLayout *layout)
{
    // 월별로 묶어서 표시
    int start_idx = 0;
    int current_month = s_view_start.month();

    for (int i = 0; i <= s_view_days; i++)
    {
        bool is_last = (i == s_view_days);
        bool month_changed = false;

        if (!is_last)
        {
            QDate d = s_view_start.addDays(i);
            month_changed = (d.month() != current_month);
        }

        if (is_last || month_changed)
        {
            int span = i - start_idx;

            QLabel *month_label = new QLabel(QString::number(current_month) + "월", this);
            month_label->setAlignment(Qt::AlignCenter);
            month_label->setStyleSheet("background-color: white; "
                                       "font-size: 11px; "
                                       "color: #5F6469; "
                                       "border-right: 1px solid #E5E8EB; "
                                       "border-bottom: 1px solid #E5E8EB;");

            // 똑같이 span 넣어서 쭉 늘려주기 (stretch)
            layout->addWidget(month_label, span);

            // 월 바뀌면 초기화
            if (month_changed)
            {
                start_idx = i;
                current_month = s_view_start.addDays(i).month();
            }
        }
    }
}

void Date_Header::build_day_row(QHBoxLayout *layout)
{
    // 날짜별로 찍어주는 코드
    QDate today = QDate::currentDate();
    QStringList day_names = { "월", "화", "수", "목", "금", "토", "일" };

    for (int i = 0; i < s_view_days; i++)
    {
        QDate current_date = s_view_start.addDays(i);
        int day_of_week = current_date.dayOfWeek();
        bool is_weekend = (day_of_week == 6 || day_of_week == 7);
        bool is_today = (current_date == today);

        // 요일 먼저 찍고, 그 아래에 날짜
        QString text = QString("%1\n%2").arg(day_names[day_of_week - 1]).arg(current_date.day());

        QLabel *cell = new QLabel(text, this);
        cell->setAlignment(Qt::AlignCenter);

        QString style = "background-color: white; "
                        "font-size: 10px; "
                        "color: #8A8F95; "
                        "border-right: 1px solid #F0F2F5; "
                        "border-bottom: 1px solid #E5E8EB;";

        if (is_weekend)
        {
            style.replace("background-color: white;", "background-color: #F5F7FA;");
        }

        if (is_today)
        {
            style = "background-color: #FFF5F5; "
                    "font-size: 10px; "
                    "font-weight: bold; "
                    "color: #E24B4A; "
                    "border-right: 1px solid #F0F2F5; "
                    "border-bottom: 2px solid #E24B4A;";
        }

        cell->setStyleSheet(style);
        layout->addWidget(cell);
    }
}