/***********************projec
 * 04/20 MON
 * main_page.cpp header file
 * Created By 방준한
 * Version 1.0
 ***********************
 */
#include "main_page.h"
#include "ui_main_page.h"
#include <QListView>
#include "schedule_bar.h"

main_page::main_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_page)
{
    ui->setupUi(this);
    ui->project_combobox->setView(new QListView());

    //
    timeline_layout = new QVBoxLayout(ui->timeline_content_widget);
    timeline_layout->setContentsMargins(0, 0, 0, 0);
    timeline_layout->setSpacing(0);
    timeline_layout->setAlignment(Qt::AlignTop);

    load_projects();
    create_combo_box();
    // connect
    connect(ui->project_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_project_changed(int)));
    rebuild_timeline();


}


void main_page::load_projects()
{
    // 1. 파일 열기
    QFile file(schedular_json_path);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file" << file.errorString();
        return;
    }

    // 2. QByteArray로 읽고 닫기
    QByteArray raw_data = file.readAll();
    file.close();

    // 3. json으로 받기
    QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    // qDebug() << doc;

    if (doc.isNull()) {
        qWarning() << "JSON 파싱 실패:";
        return;
    }

    // 4. projects 읽어서 저장하기
    // qDebug() << doc.object();
    // qDebug() << doc.object()["projects"][0];
    // qDebug() << doc.object()["projects"][1];
    int projects_count = doc["projects"].toArray().size();
    // qDebug() << "총 프로젝트 수:" << projects_count;

    for(int i = 0; i < projects_count; i++){
        QString project_id = doc["projects"][i]["id"].toString();
        QString project_name = doc["projects"][i]["name"].toString();

        // 프로젝트 객체 만들기
        Project project(project_id, project_name);

        // 프로젝트 내에 스케쥴 갯수 세기
        int schedule_count = doc["projects"][i]["schedules"].toArray().size();
        // qDebug() << "프로젝트 이름: " << project_name << " 스케쥴 갯수: " << schedule_count;

        for(int j = 0; j < schedule_count; j ++) {
            QJsonValue scheduleData =doc["projects"][i]["schedules"][j];
            Schedule schedule(
                scheduleData["id"].toString(),
                scheduleData["title"].toString(),
                scheduleData["description"].toString(),
                QDate::fromString(scheduleData["startDate"].toString(), "yyyy-MM-dd"),
                QDate::fromString(scheduleData["endDate"].toString(), "yyyy-MM-dd"),
                scheduleData["category"].toString(),
                scheduleData["progress"].toInt(),
                project_name
            );

            // 프로젝트 객체에 스케쥴 저장
            project.add_schedule(schedule);
        }

        // 멤버 변수에 프로젝트 저장
        projects.append(project);
    }


    // qDebug() << projects[0].get_name() << projects[0].get_schedule_count();

    // schedule 중 가장 빠른 이른 날짜와 가장 늦은 날짜 찾기.
    calculate_all_dates();

    // qDebug() << "표시 범위:" << view_start_date << "~" << latest
    //         << "(" << view_days << "일)";
}

// 콤보박스 설정 함수
// 시작은 모든 프로젝트 보여주고, 콤보 박스 선택 시 프로젝트 별로 바뀌게.
void main_page::create_combo_box(){
    // qDebug() << projects.size();
    ui->project_combobox->addItem("모든 프로젝트"); // 시간 부족으로 구현 못할듯 ㅠ
    for(int i = 0; i < projects.size(); i++) {
        ui->project_combobox->addItem(projects[i].get_name());
         // qDebug() << projects[i].get_name();
    }
}

void main_page::render_project_timeline(Project &project)
{
    QList<Schedule> schedules = project.get_schedules();
    qDebug() << "=== render_project_timeline 호출됨! ===";

    for (Schedule &s : schedules) {
        Schedule_Bar *bar = new Schedule_Bar(s, view_start_date, view_days, this);
        timeline_layout->addWidget(bar);
    }
}
void main_page::on_project_changed(int index)
{
    qDebug() << "콤보박스 변경됨! 인덱스:" << index;

    // ⭐ 인덱스 0 처리 분기
    if (index == 0) {
        qDebug() << "모든 프로젝트 선택, projects size:" << projects.size();
    } else {
        qDebug() << projects[index - 1].get_name()
        << "projects size" << projects.size();
    }

    rebuild_timeline();
}

void main_page::calculate_all_dates() {
    QDate earliest, latest;

    for (Project &project : projects) {
        for (Schedule &schedule : project.get_schedules()) {
            if (!earliest.isValid() || schedule.get_start_date() < earliest) {
                earliest = schedule.get_start_date();
            }
            if (!latest.isValid() || schedule.get_end_date() > latest) {
                latest = schedule.get_end_date();
            }
        }
    }

    view_start_date = earliest;
    view_end_date = latest;
    view_days = earliest.daysTo(latest) + 1;

    // qDebug() << "전체 범위:" << view_start_date << "~" << view_end_date
    //          << "(" << view_days << "일)";
}


void main_page::recalculate_view_dates(Project &project) {
    QDate earliest, latest;
    for (Schedule &schedule : project.get_schedules()){
        if (!earliest.isValid() || schedule.get_start_date() < earliest) {
            earliest = schedule.get_start_date();
        }
        if (!latest.isValid() || schedule.get_end_date() > latest) {
            latest = schedule.get_end_date();
        }
    }


    view_start_date = earliest;
    view_end_date = latest;
    view_days = earliest.daysTo(latest) + 1;
}

// function to re_draw the timeline layout
void main_page::rebuild_timeline()
{
    int index = ui->project_combobox->currentIndex();
    qDebug() << "1. rebuild_timeline 시작, index:" << index;
    clear_timeline();
    qDebug() << "2. clear_timeline 완료";

    if (index == 0) {
        calculate_all_dates();
        // ⭐ "모든 프로젝트" 처리 (여기 비어있었음!)
        QString project_name = "모든 프로젝트";
        qDebug() << "3. 모든 프로젝트 선택";
        qDebug() << "6. view_start_date:" << view_start_date << "view_days:" << view_days;
        date_header = new Date_Header(project_name, view_start_date, view_days, this);
        qDebug() << "7. Date_Header 생성 완료";

        timeline_layout->addWidget(date_header);
        qDebug() << "8. addWidget 완료";

        render_all_projects();

    } else {
        // 특정 프로젝트 선택
        int project_idx = index - 1;
        qDebug() << "3. project_idx:" << project_idx << "projects.size():" << projects.size();

        if (project_idx >= projects.size()) {
            qDebug() << "⚠️ 범위 초과!";
            return;
        }

        Project selected = projects[project_idx];
        qDebug() << "4. selected 가져옴:" << selected.get_name();

        QString project_name = selected.get_name();
        qDebug() << "5. project_name:" << project_name;
        qDebug() << "6. view_start_date:" << view_start_date << "view_days:" << view_days;
        recalculate_view_dates(selected);
        date_header = new Date_Header(project_name, view_start_date, view_days, this);
        qDebug() << "7. Date_Header 생성 완료";

        timeline_layout->addWidget(date_header);
        qDebug() << "8. addWidget 완료";

        render_project_timeline(selected);
        qDebug() << "9. render_project_timeline 완료";
    }
}
// function to clear all layouts in the timeline layout
void main_page::clear_timeline()
{
    // 레이아웃에서 모든 위젯 제거 + 메모리 해제
    QLayoutItem *item;
    while ((item = timeline_layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void main_page::render_all_projects()
{
    for (Project &project : projects) {
        QList<Schedule> schedules = project.get_schedules();

        for (Schedule &s : schedules) {
            Schedule_Bar *bar = new Schedule_Bar(s, view_start_date, view_days, this);
            timeline_layout->addWidget(bar);
        }
    }
}

main_page::~main_page()
{
    delete ui;
}
