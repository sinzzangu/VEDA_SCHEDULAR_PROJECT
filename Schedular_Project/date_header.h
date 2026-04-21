/***********************
 * 04/20 MON
 * date_header.cpp
 * date_header class for timeline
 * 프로젝트마다 헤더(날짜) 기간 잡아서 위에 보여주기.
 * Created By 방준한
 * Version 4.0
 * 프로젝트 이름 받아서 왼쪽 상단에 배치
 * 프로젝트마다 헤더(날짜) 기간 잡아서 위에 보여주기.
 * 프로젝트마다 년, 월, 일 계산해서 레이어 쌓아서 이쁘게 보여주기
 ***********************
 */
#ifndef DATE_HEADER_H
#define DATE_HEADER_H

#include <QWidget>
#include <QDate>

class QLabel;
class QHBoxLayout;

class Date_Header : public QWidget
{
    Q_OBJECT
public:
    Date_Header(QString &project_name,
                QDate &view_start,
                int view_days,
                QWidget *parent = nullptr);



private:
    void build_year_row(QHBoxLayout *layout);
    void build_month_row(QHBoxLayout *layout);
    void build_day_row(QHBoxLayout *layout);
    void build_cells();

    QString s_project_name;
    QLabel *s_project_label;
    QDate s_view_start;
    int s_view_days;
    QHBoxLayout *s_layout; // default box layout (맨 왼쪽에 project_name, 그 옆에 년도 칸)
};

#endif