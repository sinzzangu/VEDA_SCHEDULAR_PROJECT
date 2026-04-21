/***********************projec
 * created: 04/20 MON
 * main_page.cpp
 * Created By 방준한
 * Version 3.0
 *
 ***********************
 */
#include "main_page.h"
#include "ui_main_page.h"
#include <QListView>
#include "schedule_bar.h"
#include <QMessageBox>

main_page::main_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_page)
{
    // 기본 UI setup
    ui->setupUi(this);
    // 다크모드에서 콤보박스 잘 안여서 view로 보이게 하기
    ui->project_combobox->setView(new QListView());
    // 현재 날짜 출력해주기
    QDateTime stCurrentDateTime = QDateTime::currentDateTime();
    ui->page_subtitle_label->setText(stCurrentDateTime.toString("yyyy-MM-dd dddd"));

    // timeline_layout 만들기
    timeline_layout = new QVBoxLayout(ui->timeline_content_widget);
    timeline_layout->setContentsMargins(0, 0, 0, 0);
    timeline_layout->setSpacing(0);
    timeline_layout->setAlignment(Qt::AlignTop);

    load_projects();
    create_combo_box();

    // connect
    connect(ui->project_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(handle_project_changed(int)));
    // connect 일정 추가 버튼
    connect(ui->add_schedule_button, SIGNAL(clicked()), this, SLOT(handle_add_schedule_button_clicked()));
    // connect 프로젝트 추가
    connect(ui->add_project_button, SIGNAL(clicked()), this, SLOT(handle_add_project_button_clicked()));
    rebuild_timeline();

    save_projects_to_json();


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

void main_page::render_selected_project(Project &project)
{
    QList<Schedule> schedules = project.get_schedules();
    qDebug() << "render_project_timeline function called";

    for (Schedule &s : schedules) {
        Schedule_Bar *bar = new Schedule_Bar(s, view_start_date, view_days, this);
        // connector for signal
        connect(bar, SIGNAL(schedule_clicked(Schedule)), this, SLOT(handle_schedule_clicked(Schedule)));
        timeline_layout->addWidget(bar);
    }
}

void main_page::handle_project_changed(int index)
{
    rebuild_timeline();
    // qDebug() << "combo box changed" << index;
    // if index is 0 -> have to show all projects
    // if (index == 0) {
    //     qDebug() << "모든 프로젝트 선택, projects size:" << projects.size();
    // } else {
    //     // show only selected project
    //     qDebug() << projects[index - 1].get_name()
    //     << "projects size" << projects.size();
    // }
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

    // ⭐ 빈 프로젝트 방어: 스케줄이 없으면 날짜 기본값 설정
    if (!earliest.isValid() || !latest.isValid()) {
        QDate today = QDate::currentDate();
        view_start_date = today;
        view_end_date = today.addDays(14);
        view_days = 15;
        qDebug() << "⚠️ 빈 프로젝트 - 기본 날짜 범위 사용";
        return;
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

    // ⭐ 빈 프로젝트 방어: 스케줄이 없으면 날짜 기본값 설정
    if (!earliest.isValid() || !latest.isValid()) {
        QDate today = QDate::currentDate();
        view_start_date = today;
        view_end_date = today.addDays(14);
        view_days = 15;
        qDebug() << "⚠️ 빈 프로젝트 - 기본 날짜 범위 사용";
        return;
    }

    view_start_date = earliest;
    view_end_date = latest;
    view_days = earliest.daysTo(latest) + 1;
}

// function to re_draw the timeline layout
void main_page::rebuild_timeline()
{
    // 1. get combo box index and build accordingly.
    int index = ui->project_combobox->currentIndex();
    qDebug() << "1. rebuild_timeline start, index:" << index;
    // clear the timeline layout
    clear_timeline();
    qDebug() << "2. clear_timeline done";

    if (index == 0) {
        // need to re_calculate dates for all projects
        calculate_all_dates();

        QString project_name = "모든 프로젝트";
        qDebug() << "6. view_start_date:" << view_start_date << "view_days:" << view_days;
        // create new header
        date_header = new Date_Header(project_name, view_start_date, view_days, this);
        qDebug() << "7. new date header created";

        timeline_layout->addWidget(date_header);
        render_all_projects();

    } else {

        // if project selected
        int project_idx = index - 1; // index -1 as index 0 is for all projects
        qDebug() << "3. project_idx:" << project_idx << "projects.size():" << projects.size();

        if (project_idx >= projects.size()) {
            qDebug() << "project index out of bounds error";
            return;
        }

        Project selected = projects[project_idx];
        qDebug() << "4. selected project:" << selected.get_name();

        QString project_name = selected.get_name();
        qDebug() << "5. project_name:" << project_name;
        qDebug() << "6. view_start_date:" << view_start_date << "view_days:" << view_days;

        // need to recalculate dates for the specific project
        recalculate_view_dates(selected);
        date_header = new Date_Header(project_name, view_start_date, view_days, this);
        qDebug() << "7. Date_Header created";

        timeline_layout->addWidget(date_header);
        render_selected_project(selected);
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

// function to create all projects
void main_page::render_all_projects()
{
    for (Project &project : projects) {
        QList<Schedule> schedules = project.get_schedules();
        render_selected_project(project);
        // for (Schedule &s : schedules) {
        //     Schedule_Bar *bar = new Schedule_Bar(s, view_start_date, view_days, this);
        //     timeline_layout->addWidget(bar);
        // }
    }
}


// schedule functions
// 수정된 스케줄을 projects 리스트 안에서 교체
void main_page::update_schedule_in_projects(Schedule &updated)
{
    QString target_id = updated.get_id();
    qDebug() << "업데이트 시도 - id:" << target_id;

    for (int i = 0; i < projects.size(); i++) {
        // 해당 프로젝트에 있는지 확인
        QList<Schedule> schedules = projects[i].get_schedules();
        for (int j = 0; j < schedules.size(); j++) {
            if (schedules[j].get_id() == target_id) {
                projects[i].replace_schedule_by_id(target_id, updated);
                qDebug() << "업데이트 완료! 프로젝트:" << projects[i].get_name();
                return;   // ⭐ 찾았으니 끝
            }
        }
    }

    qDebug() << "cannot find schedule";
}

// id로 스케줄을 projects 리스트에서 삭제
void main_page::delete_schedule_from_projects(QString id)
{
    qDebug() << "삭제 시도 - id:" << id;

    // 모든 프로젝트 루프 돌며 삭제
    for (int i = 0; i < projects.size(); i++) {
        projects[i].remove_schedule_by_id(id);
    }

    qDebug() << "삭제 완료!";
}

// projects 리스트를 JSON 파일로 저장
void main_page::save_projects_to_json()
{
    // 1. 최상위 JSON 객체와 projects 배열 준비
    QJsonObject root_object;
    QJsonArray projects_array;

    // 2. 각 프로젝트를 JSON 객체로 변환
    for (int i = 0; i < projects.size(); i++) {
        QJsonObject project_object;
        project_object["id"] = projects[i].get_id();
        project_object["name"] = projects[i].get_name();

        // 2-1. 각 스케줄을 JSON 객체로 변환
        QJsonArray schedules_array;
        QList<Schedule> schedules = projects[i].get_schedules();

        for (int j = 0; j < schedules.size(); j++) {
            QJsonObject schedule_object;
            schedule_object["id"] = schedules[j].get_id();
            schedule_object["title"] = schedules[j].get_title();
            schedule_object["description"] = schedules[j].get_description();
            schedule_object["startDate"] = schedules[j].get_start_date().toString("yyyy-MM-dd");
            schedule_object["endDate"] = schedules[j].get_end_date().toString("yyyy-MM-dd");
            schedule_object["category"] = schedules[j].get_category();
            schedule_object["progress"] = schedules[j].get_progress();

            schedules_array.append(schedule_object);
        }

        project_object["schedules"] = schedules_array;
        projects_array.append(project_object);
    }

    // 3. root_object에 projects 배열 넣기
    root_object["projects"] = projects_array;

    // 4. QJsonDocument로 감싸기
    QJsonDocument doc(root_object);

    // 5. 파일에 쓰기
    QFile file(schedular_json_path);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "⚠️ JSON 파일 쓰기 실패:" << file.errorString();
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "✅ JSON 저장 완료!";
}

// slot functions
void main_page::handle_schedule_clicked(Schedule schedule)
{
    qDebug() << "=== Schedule 클릭됨 ===";
    qDebug() << "제목:" << schedule.get_title();

    Schedule_Dialog dialog(schedule, projects, this);;
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        // ⭐ 삭제인지 수정인지 구분
        if (dialog.is_deleted()) {
            qDebug() << "→ 삭제 처리";
            QString id = schedule.get_id();
            delete_schedule_from_projects(id);
        } else {
            qDebug() << "→ 수정 처리";
            Schedule updated = dialog.get_updated_schedule();
            update_schedule_in_projects(updated);
        }

        // ⭐ 파일에 저장
        save_projects_to_json();

        // ⭐ 화면 다시 그리기
        rebuild_timeline();

    } else {
        qDebug() << "취소됨";
    }
}

// ⭐ 고유 id 생성
QString main_page::generate_schedule_id()
{
    qint64 timestamp_ms = QDateTime::currentMSecsSinceEpoch();
    QString id = "sch_" + QString::number(timestamp_ms);
    return id;
}

// ⭐ 특정 프로젝트에 스케줄 추가
void main_page::add_schedule_to_project(QString &project_name, Schedule &new_schedule)
{
    for (int i = 0; i < projects.size(); i++) {
        if (projects[i].get_name() == project_name) {
            projects[i].add_schedule(new_schedule);
            qDebug() << "일정 추가 완료 - 프로젝트:" << project_name
                     << "제목:" << new_schedule.get_title();
            return;
        }
    }
    qDebug() << "⚠️ 프로젝트를 찾을 수 없음:" << project_name;
}

// ⭐ + 일정 버튼 클릭 핸들러
void main_page::handle_add_schedule_button_clicked()
{
    qDebug() << "=== + 일정 버튼 클릭됨 ===";

    // 현재 보고 있는 프로젝트 이름 가져오기 (신규 다이얼로그 기본 선택용)
    QString default_project_name;
    int current_idx = ui->project_combobox->currentIndex();
    if (current_idx == 0) {
        // "모든 프로젝트" 상태 → 첫 번째 실제 프로젝트를 기본값으로
        if (projects.size() > 0) {
            default_project_name = projects[0].get_name();
        }
    } else {
        default_project_name = projects[current_idx - 1].get_name();
    }

    // 신규 모드 다이얼로그
    Schedule_Dialog dialog(projects, default_project_name, this);
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        Schedule new_schedule = dialog.get_updated_schedule();
        QString selected_project = dialog.get_selected_project_name();

        // id 채우기
        QString new_id = generate_schedule_id();
        new_schedule.set_id(new_id);  // ⭐ Schedule에 set_id 있어야 함

        qDebug() << "→ 신규 일정 생성, id:" << new_id
                 << "프로젝트:" << selected_project;

        // 해당 프로젝트에 추가
        add_schedule_to_project(selected_project, new_schedule);

        // JSON 저장 + 화면 다시 그리기
        save_projects_to_json();
        rebuild_timeline();
    } else {
        qDebug() << "신규 일정 추가 취소됨";
    }
}

// 프로젝트 추가 관련 함수들
// 프로젝트 ID 생성 함수
QString main_page::generate_project_id() {
    int timestamp_ms = QDateTime::currentMSecsSinceEpoch();
    QString id = "proj_" + QString::number(timestamp_ms);

    return id;
}

// check for duplicte project name
bool main_page::is_project_name_duplicate(QString &name){
    for(int i = 0; i < projects.size(); i++){
        if (projects[i].get_name() == name) {
            return true;
        }
    }
    return false;
}

// slot function for add project button
void main_page::handle_add_project_button_clicked()
{
    qDebug() << "=== 프로젝트 관리 버튼 클릭됨 ===";

    Project_Dialog dialog(projects, this);

    // 추가 요청 시그널 연결
    connect(&dialog, SIGNAL(project_add_requested(QString)),
            this, SLOT(handle_project_add_requested(QString)));

    // 삭제 요청 시그널 연결
    connect(&dialog, SIGNAL(project_delete_requested(QString)),
            this, SLOT(handle_project_delete_requested(QString)));

    // 다이얼로그가 refresh 받도록 멤버로 저장
    s_active_project_dialog = &dialog;

    dialog.exec();

    s_active_project_dialog = nullptr;
}


void main_page::handle_project_add_requested(QString name)
{
    qDebug() << "→ 프로젝트 추가 요청:" << name;

    // 중복 체크 (다이얼로그에서도 했지만 이중 안전장치)
    if (is_project_name_duplicate(name)) {
        qDebug() << "⚠️ 중복 프로젝트 이름:" << name;
        return;
    }

    // 새 프로젝트 생성
    QString new_id = generate_project_id();
    Project new_project(new_id, name);
    projects.append(new_project);

    // JSON 저장
    save_projects_to_json();

    // 메인 콤보박스 갱신 + 새 프로젝트로 자동 전환
    ui->project_combobox->blockSignals(true);
    ui->project_combobox->clear();
    create_combo_box();
    int new_project_combo_idx = projects.size();  // 맨 마지막
    ui->project_combobox->setCurrentIndex(new_project_combo_idx);
    ui->project_combobox->blockSignals(false);

    rebuild_timeline();

    // 다이얼로그에 refresh
    if (s_active_project_dialog) {
        s_active_project_dialog->refresh_project_list(projects);
    }
}

void main_page::handle_project_delete_requested(QString project_id)
{
    qDebug() << "→ 프로젝트 삭제 요청:" << project_id;

    // 삭제 대상 찾기
    int target_idx = -1;
    QString deleted_name;
    for (int i = 0; i < projects.size(); i++) {
        if (projects[i].get_id() == project_id) {
            target_idx = i;
            deleted_name = projects[i].get_name();
            break;
        }
    }

    if (target_idx < 0) {
        qDebug() << "⚠️ 삭제할 프로젝트를 찾을 수 없음";
        return;
    }

    // 현재 메인 콤보박스가 이 프로젝트를 보고 있었는지 확인
    int current_combo_idx = ui->project_combobox->currentIndex();
    bool was_viewing_deleted = (current_combo_idx - 1 == target_idx);

    // 실제 삭제
    projects.removeAt(target_idx);

    // JSON 저장
    save_projects_to_json();

    // 메인 콤보박스 갱신
    ui->project_combobox->blockSignals(true);
    ui->project_combobox->clear();
    create_combo_box();

    if (was_viewing_deleted) {
        // 보고 있던 프로젝트가 삭제됨 → "모든 프로젝트"로
        ui->project_combobox->setCurrentIndex(0);
    } else if (current_combo_idx == 0) {
        // "모든 프로젝트" 상태 유지
        ui->project_combobox->setCurrentIndex(0);
    } else if (current_combo_idx - 1 > target_idx) {
        // 삭제된 프로젝트가 앞에 있어서 인덱스가 밀림 → 하나 줄이기
        ui->project_combobox->setCurrentIndex(current_combo_idx - 1);
    } else {
        // 삭제된 프로젝트가 뒤에 있음 → 그대로
        ui->project_combobox->setCurrentIndex(current_combo_idx);
    }
    ui->project_combobox->blockSignals(false);

    rebuild_timeline();

    // 다이얼로그 refresh
    if (s_active_project_dialog) {
        s_active_project_dialog->refresh_project_list(projects);
    }

    qDebug() << "프로젝트 삭제 완료:" << deleted_name;
}
main_page::~main_page()
{
    delete ui;
}
