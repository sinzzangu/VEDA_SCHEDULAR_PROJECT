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

    void set_project_name(QString &name);

private:
    void build_cells();

    QString s_project_name;
    QLabel *s_project_label;
    QDate s_view_start;
    int s_view_days;
    QHBoxLayout *s_layout;
};

#endif