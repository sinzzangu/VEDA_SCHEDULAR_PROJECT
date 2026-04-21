/***********************
 * 04/21 MON
 * main_page.cpp
 * Created By 방준한
 * Version 6.0
 * 리팩토링: Schedule_Manager 분리
 ***********************
 */
#include "main_page.h"
#include "ui_main_page.h"
#include <QListView>
#include <QDateTime>
#include "schedule_bar.h"

main_page::main_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_page)
{
    // 기본 UI setup
    ui->setupUi(this);
    ui->project_combobox->setView(new QListView());

    // 현재 날짜 출력
    QDateTime stCurrentDateTime = QDateTime::currentDateTime();
    ui->page_subtitle_label->setText(stCurrentDateTime.toString("yyyy-MM-dd dddd"));

    // timeline_layout 만들기
    timeline_layout = new QVBoxLayout(ui->timeline_content_widget);
    timeline_layout->setContentsMargins(0, 0, 0, 0);
    timeline_layout->setSpacing(0);
    timeline_layout->setAlignment(Qt::AlignTop);

    // ⭐ Schedule_Manager 생성 및 데이터 로드
    schedule_manager = new Schedule_Manager(this);
    schedule_manager->load_projects();

    create_combo_box();

    // 버튼/콤보박스 연결
    connect(ui->project_combobox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(handle_project_changed(int)));
    connect(ui->add_schedule_button, SIGNAL(clicked()),
            this, SLOT(handle_add_schedule_button_clicked()));
    connect(ui->add_project_button, SIGNAL(clicked()),
            this, SLOT(handle_add_project_button_clicked()));

    rebuild_timeline();
    schedule_manager->save_projects_to_json();
}

main_page::~main_page()
{
    delete ui;
}

// 콤보박스에 "모든 프로젝트" + 각 프로젝트 이름 채우기
void main_page::create_combo_box()
{
    ui->project_combobox->addItem("모든 프로젝트");
    QList<Project> &projects = schedule_manager->get_projects();
    for (int i = 0; i < projects.size(); i++) {
        ui->project_combobox->addItem(projects[i].get_name());
    }
}

void main_page::render_selected_project(Project &project)
{
    QList<Schedule> schedules = project.get_schedules();
    qDebug() << "render_project_timeline function called";

    for (Schedule &s : schedules) {
        Schedule_Bar *bar = new Schedule_Bar(s, view_start_date, view_days, this);
        connect(bar, SIGNAL(schedule_clicked(Schedule)),
                this, SLOT(handle_schedule_clicked(Schedule)));
        timeline_layout->addWidget(bar);
    }
}

void main_page::handle_project_changed(int index)
{
    Q_UNUSED(index);
    rebuild_timeline();
}

void main_page::calculate_all_dates()
{
    QDate earliest, latest;
    QList<Project> &projects = schedule_manager->get_projects();

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

void main_page::recalculate_view_dates(Project &project)
{
    QDate earliest, latest;
    for (Schedule &schedule : project.get_schedules()) {
        if (!earliest.isValid() || schedule.get_start_date() < earliest) {
            earliest = schedule.get_start_date();
        }
        if (!latest.isValid() || schedule.get_end_date() > latest) {
            latest = schedule.get_end_date();
        }
    }

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

void main_page::rebuild_timeline()
{
    int index = ui->project_combobox->currentIndex();
    qDebug() << "1. rebuild_timeline start, index:" << index;
    clear_timeline();
    qDebug() << "2. clear_timeline done";

    QList<Project> &projects = schedule_manager->get_projects();

    if (index == 0) {
        calculate_all_dates();

        QString project_name = "모든 프로젝트";
        qDebug() << "6. view_start_date:" << view_start_date << "view_days:" << view_days;
        date_header = new Date_Header(project_name, view_start_date, view_days, this);
        qDebug() << "7. new date header created";

        timeline_layout->addWidget(date_header);
        render_all_projects();

    } else {
        int project_idx = index - 1;
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

        recalculate_view_dates(selected);
        date_header = new Date_Header(project_name, view_start_date, view_days, this);
        qDebug() << "7. Date_Header created";

        timeline_layout->addWidget(date_header);
        render_selected_project(selected);
        qDebug() << "9. render_project_timeline 완료";
    }
}

void main_page::clear_timeline()
{
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
    QList<Project> &projects = schedule_manager->get_projects();
    for (Project &project : projects) {
        render_selected_project(project);
    }
}

// 스케줄 클릭 → 수정/삭제 다이얼로그
void main_page::handle_schedule_clicked(Schedule schedule)
{
    qDebug() << "=== Schedule 클릭됨 ===";
    qDebug() << "제목:" << schedule.get_title();

    QList<Project> &projects = schedule_manager->get_projects();
    Schedule_Dialog dialog(schedule, projects, this);
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        if (dialog.is_deleted()) {
            qDebug() << "→ 삭제 처리";
            QString id = schedule.get_id();
            schedule_manager->delete_schedule_from_projects(id);
        } else {
            qDebug() << "→ 수정 처리";
            Schedule updated = dialog.get_updated_schedule();
            schedule_manager->update_schedule_in_projects(updated);
        }

        schedule_manager->save_projects_to_json();
        rebuild_timeline();

    } else {
        qDebug() << "취소됨";
    }
}

// + 일정 버튼 클릭 → 신규 모드 다이얼로그
void main_page::handle_add_schedule_button_clicked()
{
    qDebug() << "=== + 일정 버튼 클릭됨 ===";

    QList<Project> &projects = schedule_manager->get_projects();

    // 현재 보고 있는 프로젝트 이름 (신규 다이얼로그 기본 선택용)
    QString default_project_name;
    int current_idx = ui->project_combobox->currentIndex();
    if (current_idx == 0) {
        if (projects.size() > 0) {
            default_project_name = projects[0].get_name();
        }
    } else {
        default_project_name = projects[current_idx - 1].get_name();
    }

    Schedule_Dialog dialog(projects, default_project_name, this);
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        Schedule new_schedule = dialog.get_updated_schedule();
        QString selected_project = dialog.get_selected_project_name();

        QString new_id = schedule_manager->generate_schedule_id();
        new_schedule.set_id(new_id);

        qDebug() << "→ 신규 일정 생성, id:" << new_id
                 << "프로젝트:" << selected_project;

        schedule_manager->add_schedule_to_project(selected_project, new_schedule);
        schedule_manager->save_projects_to_json();
        rebuild_timeline();
    } else {
        qDebug() << "신규 일정 추가 취소됨";
    }
}

// + 프로젝트 (관리) 버튼 클릭
void main_page::handle_add_project_button_clicked()
{
    qDebug() << "=== 프로젝트 관리 버튼 클릭됨 ===";

    QList<Project> &projects = schedule_manager->get_projects();
    Project_Dialog dialog(projects, this);

    connect(&dialog, SIGNAL(project_add_requested(QString)),
            this, SLOT(handle_project_add_requested(QString)));
    connect(&dialog, SIGNAL(project_delete_requested(QString)),
            this, SLOT(handle_project_delete_requested(QString)));

    s_active_project_dialog = &dialog;
    dialog.exec();
    s_active_project_dialog = nullptr;
}

// 프로젝트 추가 요청
void main_page::handle_project_add_requested(QString name)
{
    qDebug() << "→ 프로젝트 추가 요청:" << name;

    if (schedule_manager->is_project_name_duplicate(name)) {
        qDebug() << "⚠️ 중복 프로젝트 이름:" << name;
        return;
    }

    QString new_id = schedule_manager->generate_project_id();
    Project new_project(new_id, name);
    schedule_manager->add_project(new_project);
    schedule_manager->save_projects_to_json();

    // 메인 콤보박스 갱신 + 새 프로젝트로 자동 전환
    ui->project_combobox->blockSignals(true);
    ui->project_combobox->clear();
    create_combo_box();
    int new_project_combo_idx = schedule_manager->get_project_count();
    ui->project_combobox->setCurrentIndex(new_project_combo_idx);
    ui->project_combobox->blockSignals(false);

    rebuild_timeline();

    if (s_active_project_dialog) {
        QList<Project> &projects = schedule_manager->get_projects();
        s_active_project_dialog->refresh_project_list(projects);
    }
}

// 프로젝트 삭제 요청
void main_page::handle_project_delete_requested(QString project_id)
{
    qDebug() << "→ 프로젝트 삭제 요청:" << project_id;

    QList<Project> &projects = schedule_manager->get_projects();

    // 삭제 대상 찾기 + 현재 보고 있던 프로젝트인지 확인
    int target_idx = -1;
    for (int i = 0; i < projects.size(); i++) {
        if (projects[i].get_id() == project_id) {
            target_idx = i;
            break;
        }
    }

    if (target_idx < 0) {
        qDebug() << "⚠️ 삭제할 프로젝트를 찾을 수 없음";
        return;
    }

    int current_combo_idx = ui->project_combobox->currentIndex();
    bool was_viewing_deleted = (current_combo_idx - 1 == target_idx);

    // 실제 삭제
    schedule_manager->delete_project_by_id(project_id);
    schedule_manager->save_projects_to_json();

    // 메인 콤보박스 갱신
    ui->project_combobox->blockSignals(true);
    ui->project_combobox->clear();
    create_combo_box();

    if (was_viewing_deleted) {
        ui->project_combobox->setCurrentIndex(0);
    } else if (current_combo_idx == 0) {
        ui->project_combobox->setCurrentIndex(0);
    } else if (current_combo_idx - 1 > target_idx) {
        ui->project_combobox->setCurrentIndex(current_combo_idx - 1);
    } else {
        ui->project_combobox->setCurrentIndex(current_combo_idx);
    }
    ui->project_combobox->blockSignals(false);

    rebuild_timeline();

    if (s_active_project_dialog) {
        QList<Project> &projects_after = schedule_manager->get_projects();
        s_active_project_dialog->refresh_project_list(projects_after);
    }
}