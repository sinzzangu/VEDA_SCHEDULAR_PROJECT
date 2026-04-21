/***********************************************************************
 * 04/21 MON
 * main_page.cpp
 *
 * [역할]
 *   앱의 메인 화면을 담당하는 컨트롤러.
 *   사용자가 보는 간트 차트 기반 스케줄러의 최상위 윈도우이며,
 *   각 UI 이벤트를 받아 적절한 매니저/다이얼로그로 위임·조율함.
 *
 * [주요 기능]
 *   - Schedule_Manager를 통해 프로젝트/스케줄 데이터 로딩 및 저장 위임
 *   - 간트 차트 렌더링(date_header + Schedule_Bar 생성·배치)
 *   - 프로젝트 필터 콤보박스 관리 및 선택 변경 반영
 *   - 스케줄 수정/삭제/추가 다이얼로그 호출 및 결과 처리
 *   - 프로젝트 추가/삭제 다이얼로그와 시그널·슬롯으로 연동
 *   - 로그아웃 확인 UI 제공 (실제 로그아웃 로직은 통합 시 연결 예정)
 *
 * Created By 방준한
 * Version 6.0 (Schedule_Manager 분리 리팩토링 반영)
 ***********************************************************************/
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

    // timeline_layout 만들기 (여기에 간트 차트가 들어감)
    timeline_layout = new QVBoxLayout(ui->timeline_content_widget);
    timeline_layout->setContentsMargins(0, 0, 0, 0);
    timeline_layout->setSpacing(0);
    timeline_layout->setAlignment(Qt::AlignTop);

    // 스케쥴 매니저로 백엔드 느낌으로 모든 데이터 처리.
    schedule_manager = new Schedule_Manager(this);
    // 데이터 읽어오기
    schedule_manager->load_projects();

    // 콤보박스 생성
    create_combo_box();

    // 버튼/콤보박스 연결
    connect(ui->project_combobox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(handle_project_changed(int)));
    connect(ui->add_schedule_button, SIGNAL(clicked()),
            this, SLOT(handle_add_schedule_button_clicked()));
    connect(ui->add_project_button, SIGNAL(clicked()),
            this, SLOT(handle_add_project_button_clicked()));

    // 타임라인 레이아웃 채우기
    rebuild_timeline();
}

main_page::~main_page()
{
    delete ui;
}

// 콤보박스에 "모든 프로젝트"(default) + 각 프로젝트 이름 채우기
void main_page::create_combo_box()
{
    ui->project_combobox->addItem("모든 프로젝트");
    QList<Project> &projects = schedule_manager->get_projects();
    for (int i = 0; i < projects.size(); i++) {
        ui->project_combobox->addItem(projects[i].get_name());
    }
}

// 특정 프로젝트 선택하면, 그 프로젝트만 화면에 보이게하기
void main_page::render_selected_project(Project &project)
{
    QList<Schedule> schedules = project.get_schedules();
    // qDebug() << "render_project_timeline function called";

    // 루프 돌면서, 스케쥴마다 바 생성해서 찍어줌
    for (Schedule &s : schedules) {
        Schedule_Bar *bar = new Schedule_Bar(s, view_start_date, view_days, this);
        // schedule_bar 어디든 눌러서 수정할 수 있게 만들기.
        // schedule_clicked는 schedule_bar에 시그널로 지정해 놨음
        // mousePressEvent override 되어서 넘겨주는 형태.
        connect(bar, SIGNAL(schedule_clicked(Schedule)),
                this, SLOT(handle_schedule_clicked(Schedule)));
        timeline_layout->addWidget(bar);
    }
}

// combo box에서 프로젝트 선택하면 자동으로 콤보박스에서 텍스트랑 인덱스 읽어와서 화면 재구성하기
void main_page::handle_project_changed(int index)
{
    rebuild_timeline();
}

// 전체 프로젝트 선택시 최대, 최소 날짜 계산해서 저장해주기
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

// 특정 프로젝트 선택시 최대, 최소 날짜 계산해서 저장하기
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

// 제일 중요한 함수
// 타임라인 에리어에 알맞는 프로젝트 가져와서 그리기
void main_page::rebuild_timeline()
{
    // combo_box에서 인덱스 읽어오기
    int index = ui->project_combobox->currentIndex();
    qDebug() << "1. rebuild_timeline start, index:" << index;
    // 모든 위젯 다 지우기
    clear_timeline();
    qDebug() << "2. clear_timeline done";

    // 혹시나 index가 0 이면 모든 프로젝트 다 가져와야 하니까 일단 다 가져오기
    // 업데이트 있을 수 있으니까 다시 받아오기
    QList<Project> &projects = schedule_manager->get_projects();

    // 전체 프로젝트 선택시
    if (index == 0) {
        // 1. 표시할 날짜 다시 계산
        calculate_all_dates();

        QString project_name = "모든 프로젝트";
        // 맨 위에 date header 칸 만들어주기
        date_header = new Date_Header(project_name, view_start_date, view_days, this);
        qDebug() << "new date header created";
        // timeline_layout에 추가해주기
        timeline_layout->addWidget(date_header);
        // 전체 프로젝트 다 그리기
        render_all_projects();
    } else {
        // combo box에서 0번째는 전체 프로젝트니까 index - 1 해주기
        int project_idx = index - 1;
        qDebug() << "project_idx:" << project_idx << "projects.size():" << projects.size();

        // 혹시나 이상해지면  리턴하기
        if (project_idx >= projects.size()) {
            qDebug() << "project index out of bounds error";
            return;
        }

        // 인덱스로 프로젝트 선택하기
        Project selected = projects[project_idx];
        qDebug() << "selected project:" << selected.get_name();

        QString project_name = selected.get_name();
        qDebug() << "project_name:" << project_name;
        // 이 프로젝트에서 최소, 최대 날짜 다시 계산하기
        recalculate_view_dates(selected);
        // 날짜 구한걸로 data header 만들기
        date_header = new Date_Header(project_name, view_start_date, view_days, this);
        // layout에 추가하기
        timeline_layout->addWidget(date_header);
        // 선택 프로젝트만 보여주기
        render_selected_project(selected);
        qDebug() << "9. render_project_timeline 완료";
    }
}

// timeline_layout 비우기
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

// 모든 프로젝트 다 찍기
void main_page::render_all_projects()
{
    QList<Project> &projects = schedule_manager->get_projects();
    for (Project &project : projects) {
        render_selected_project(project);
    }
}

// 스케줄 클릭시 수정/삭제 다이얼로그 띄우기
void main_page::handle_schedule_clicked(Schedule schedule)
{
    QList<Project> &projects = schedule_manager->get_projects();
    // 프로젝트들 받아서, schedule_dialog에 객체 넘기면서 수정/삭제 dialog 띄우기
    Schedule_Dialog dialog(schedule, projects, this);
    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        // 삭제버튼 누르면
        // dialog 에 is_deleted()가 삭제 한다면 임.
        // 창을 확인으로 닫을 수도, 삭제로 닫을 수도 있음, 둘다 accepted return 함
        // 취소 눌러야지 accepted 아님
        if (dialog.is_deleted()) {
            qDebug() << "스케쥴 삭제 처리";
            QString id = schedule.get_id();
            // 데이터 처리는 무조건 schedule_manager에서 하도록.
            schedule_manager->delete_schedule_from_projects(id);
        } else {
            // 확인으로 창 닫으면   수정 임
            qDebug() << "schedule 수정 처리";
            Schedule updated = dialog.get_updated_schedule();
            schedule_manager->update_schedule_in_projects(updated);
        }

        // 변경된것들 db에 업데이트
        schedule_manager->save_projects_to_json();
        // 변경된거 기반으로 다시 그려주기~
        rebuild_timeline();

    } else {
        qDebug() << "취소됨";
    }
}

// + 일정 버튼 클릭 → 신규 모드 다이얼로그
void main_page::handle_add_schedule_button_clicked()
{
    QList<Project> &projects = schedule_manager->get_projects();

    // 콤보박스에서 지금 내가 보고 있는 프로젝트 이름 가져오기
    QString default_project_name;
    int current_idx = ui->project_combobox->currentIndex();
    if (current_idx == 0) {
        if (projects.size() > 0) {
            default_project_name = projects[0].get_name();
        }
    } else {
        default_project_name = projects[current_idx - 1].get_name();
    }

    // 이번에는 생성자 overloading으로 다른 생성자로 schedule_dialog class 부르기
    Schedule_Dialog dialog(projects, default_project_name, this);
    int result = dialog.exec();

    // 여긴 무조건 추가만 가능함으로 accepted 만 보면 됨
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


// + 프로젝트 관리 버튼 클릭 함수
void main_page::handle_add_project_button_clicked()
{

    // 프로젝트 생성 버튼 누르면 작동해야할것.
    QList<Project> &projects = schedule_manager->get_projects();
    Project_Dialog dialog(projects, this);
    connect(&dialog, SIGNAL(project_add_requested(QString)),
            this, SLOT(handle_project_add_requested(QString)));
    connect(&dialog, SIGNAL(project_delete_requested(QString)),
            this, SLOT(handle_project_delete_requested(QString)));

    s_active_project_dialog = &dialog;
    // 기존 프로젝트들 다 넘겨주고 (이름 검사용)
    // dialog 만들어둔거 실행.
    dialog.exec();
    s_active_project_dialog = nullptr;
}

// 프로젝트 추가 요청
void main_page::handle_project_add_requested(QString name)
{
    if (schedule_manager->is_project_name_duplicate(name)) {
        qDebug() << "중복 프로젝트 이름:" << name;
        return;
    }

    QString new_id = schedule_manager->generate_project_id();
    Project new_project(new_id, name);
    schedule_manager->add_project(new_project);
    schedule_manager->save_projects_to_json();

    // 메인 콤보박스 갱신 + 새 프로젝트로 자동 전환
    // 콤보박스를 여러번 변형해야 하니, 매번 리빌드 index_changed_signal 발생시키지 말고, 잠깐 막아두자.
    ui->project_combobox->blockSignals(true);
    ui->project_combobox->clear();
    create_combo_box();

    // 지금 추가한 프로젝트는 무조건 json 파일의 맨 끝에 위치한다.
    int new_project_combo_idx = schedule_manager->get_project_count();
    ui->project_combobox->setCurrentIndex(new_project_combo_idx);

    // 다시 콤보 박스가 시그널 읽을 수 있게 하기.
    ui->project_combobox->blockSignals(false);

    // 다시 그려주기~
    rebuild_timeline();

    // 만약에 프로젝트 추가창이 아직 열려있으면, 프로젝트 리스트 창에 즉각 업데이트 하기 위해서 refresh 해 준다.
    if (s_active_project_dialog) {
        QList<Project> &projects = schedule_manager->get_projects();
        s_active_project_dialog->refresh_project_list(projects);
    }
}

// 프로젝트 삭제 요청
void main_page::handle_project_delete_requested(QString project_id)
{
    qDebug() << "프로젝트 삭제 요청:" << project_id;

    QList<Project> &projects = schedule_manager->get_projects();

    // 삭제 대상 찾기 + 현재 보고 있던 프로젝트인지 확인
    int target_idx = -1;
    for (int i = 0; i < projects.size(); i++) {
        if (projects[i].get_id() == project_id) {
            target_idx = i;
            break;
        }
    }

    // 없으면 리턴
    if (target_idx < 0) {
        qDebug() << "삭제할 프로젝트를 찾을 수 없음";
        return;
    }

    // 내가 보고있던 프로젝트가 삭제 되었나? -> 그럼 다시 타임 에리어 다시 그려야함 ㅠ
    int current_combo_idx = ui->project_combobox->currentIndex();
    bool was_viewing_deleted = (current_combo_idx - 1 == target_idx);

    // 실제 삭제
    schedule_manager->delete_project_by_id(project_id);
    schedule_manager->save_projects_to_json();

    // 메인 콤보박스 갱신
    ui->project_combobox->blockSignals(true);
    ui->project_combobox->clear();
    create_combo_box();

    // 보던거 삭제 됐으면 초기 화면 (전체 프로젝트)
    if (was_viewing_deleted) {
        ui->project_combobox->setCurrentIndex(0);
    } else if (current_combo_idx == 0) { // 전체 화면에서 삭제했으면 머 상관 없고
        ui->project_combobox->setCurrentIndex(0);
    } else if (current_combo_idx - 1 > target_idx) { // 내가 보던 프로젝트가 뒤로 밀려버림, 콤보 박스 인덱스도 처리 해줘야함.
        ui->project_combobox->setCurrentIndex(current_combo_idx - 1);
    } else { // 내가 보던 프로젝트가 삭제된 프로젝트보다 앞에 있었으면 뭐 상관 없지
        ui->project_combobox->setCurrentIndex(current_combo_idx);
    }
    ui->project_combobox->blockSignals(false);

    rebuild_timeline();

    // 여기도 삭제된거 즉각 반영되게 .
    if (s_active_project_dialog) {
        QList<Project> &projects_after = schedule_manager->get_projects();
        s_active_project_dialog->refresh_project_list(projects_after);
    }
}

void main_page::on_logout_button_clicked()
{
    QMessageBox msg_box(this);
    msg_box.setWindowTitle("로그아웃");
    msg_box.setText("로그아웃 하시겠습니까?");
    msg_box.setIcon(QMessageBox::Question);

    QPushButton *yes_button = msg_box.addButton("확인", QMessageBox::YesRole);
    msg_box.addButton("취소", QMessageBox::NoRole);

    msg_box.setStyleSheet(
        "QMessageBox { background-color: white; }"
        "QMessageBox QLabel { color: #2C2F33; font-size: 13px; padding: 8px; }"
        "QPushButton { "
        "    background-color: white; "
        "    color: #2C2F33; "
        "    border: 1px solid #D5D8DC; "
        "    border-radius: 4px; "
        "    padding: 6px 16px; "
        "    min-width: 60px; "
        "    min-height: 20px; "
        "    font-size: 12px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #F5F7FA; "
        "    border-color: #2E4A6B; "
        "}"
        );

    msg_box.exec();

    if (msg_box.clickedButton() == yes_button) {
        qDebug() << "로그아웃 확인됨";
        // TODO: 실제 로그아웃 로직은 여기에 연결
    }
}

