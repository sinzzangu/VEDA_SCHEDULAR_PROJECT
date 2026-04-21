/***********************
 * 04/21 MON
 * schedule_dialog.cpp
 * 일정 상세 보기 및 편집 다이얼로그 구현
 * Created By 방준한
 * Version 3.1
 * 프로젝트 콤보박스 지원 추가
 ***********************
 */
#include "schedule_dialog.h"
#include "ui_schedule_dialog.h"
#include <QMessageBox>
#include <QDate>

Schedule_Dialog::Schedule_Dialog(Schedule &schedule, QList<Project> &projects, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Schedule_Dialog),
    s_schedule(schedule),
    s_projects(projects)
{
    ui->setupUi(this);

    populate_project_combobox();  // ⭐ 콤보박스 먼저 채우기
    populate_fields();            // ⭐ 그 다음 현재 값 선택
    update_progress_bar();

    // 버튼 연결
    connect(ui->save_button, SIGNAL(clicked()), this, SLOT(handle_save_clicked()));

    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(reject()));

    connect(ui->delete_button, SIGNAL(clicked()), this, SLOT(handle_delete_clicked()));

    connect(ui->start_date_edit, SIGNAL(dateChanged(QDate)), this, SLOT(handle_dates_changed()));

    connect(ui->end_date_edit, SIGNAL(dateChanged(QDate)), this, SLOT(handle_dates_changed()));
}

// 일정 생성 생성자
Schedule_Dialog::Schedule_Dialog(QList<Project> &projects, QString &default_project_name, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Schedule_Dialog),
    s_projects(projects),
    s_is_new_mode(true)
{
    ui->setupUi(this);

    // 신규 모드용 UI 조정
    setWindowTitle("새 스케쥴 추가");
    ui->dialog_title_label->setText("새 스케쥴 추가");
    ui->delete_button->hide();  // 생성 모드에선 삭제 버튼 숨김

    populate_project_combobox();
    setup_new_mode_defaults(default_project_name);
    update_progress_bar();

    // 버튼 연결 (수정 모드와 동일하지만 delete 제외)
    connect(ui->save_button, SIGNAL(clicked()), this, SLOT(handle_save_clicked()));
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->start_date_edit, SIGNAL(dateChanged(QDate)), this, SLOT(handle_dates_changed()));
    connect(ui->end_date_edit, SIGNAL(dateChanged(QDate)), this, SLOT(handle_dates_changed()));
}
Schedule_Dialog::~Schedule_Dialog()
{
    delete ui;
}

// 프로젝트 콤보박스에 프로젝트 이름들 채우기
void Schedule_Dialog::populate_project_combobox()
{
    for (int i = 0; i < s_projects.size(); i++) {
        QString project_name = s_projects[i].get_name();
        ui->project_combobox->addItem(project_name);
    }
}

void Schedule_Dialog::populate_fields()
{
    // ⭐ 현재 스케줄의 프로젝트 이름을 콤보박스에서 찾아 선택
    QString current_project_name = s_schedule.get_schedule_project_name();
    int proj_index = ui->project_combobox->findText(current_project_name);
    if (proj_index >= 0) {
        ui->project_combobox->setCurrentIndex(proj_index);
    }
    ui->project_combobox->setEnabled(false);

    ui->title_edit->setText(s_schedule.get_title());
    ui->description_edit->setText(s_schedule.get_description());
    ui->start_date_edit->setDate(s_schedule.get_start_date());
    ui->end_date_edit->setDate(s_schedule.get_end_date());

    int cat_index = ui->category_combobox->findText(s_schedule.get_category());
    if (cat_index >= 0) {
        ui->category_combobox->setCurrentIndex(cat_index);
    }
}

int Schedule_Dialog::calculate_progress()
{
    QDate start = ui->start_date_edit->date();
    QDate end = ui->end_date_edit->date();
    QDate today = QDate::currentDate();

    if (start > end) {
        return 0;
    }
    if (today < start) {
        return 0;
    }
    if (today >= end) {
        return 100;
    }

    int total_days = start.daysTo(end);
    int passed_days = start.daysTo(today);

    if (total_days == 0) {
        return 100;
    }

    return (passed_days * 100) / total_days;
}

void Schedule_Dialog::update_progress_bar()
{
    int progress = calculate_progress();
    ui->progress_bar->setValue(progress);
}

void Schedule_Dialog::handle_dates_changed()
{
    update_progress_bar();
}

void Schedule_Dialog::handle_save_clicked()
{
    QDate start = ui->start_date_edit->date();
    QDate end = ui->end_date_edit->date();

    if (start > end) {
        QMessageBox::warning(this, "입력 오류",
                             "시작일이 종료일보다 늦을 수 없습니다.");
        return;
    }

    QString new_title = ui->title_edit->text();
    if (new_title.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "제목을 입력해주세요.");
        return;
    }

    // s_schedule에 수정 내용 반영
    QString new_description = ui->description_edit->toPlainText();
    QString new_category = ui->category_combobox->currentText();
    int new_progress = calculate_progress();

    if (s_is_new_mode) {
        // 신규 모드: Schedule 객체 새로 만들어서 s_schedule에 넣기
        // id와 project_name은 main_page에서 채울 수도 있지만,
        // 여기서 project_name은 알 수 있으니 넣어줌 (id는 main_page에서)
        QString selected_project = ui->project_combobox->currentText();
        // 빈 id로 Schedule 생성 → main_page에서 id 채워서 저장
        Schedule new_schedule(
            "",  // id는 main_page가 generate_schedule_id()로 채움
            new_title,
            new_description,
            start,
            end,
            new_category,
            new_progress,
            selected_project
            );
        s_schedule = new_schedule;

    } else {
        // 수정 모드: 기존 s_schedule 값 업데이트
        s_schedule.set_title(new_title);
        s_schedule.set_description(new_description);
        s_schedule.set_start_date(start);
        s_schedule.set_end_date(end);
        s_schedule.set_category(new_category);
        s_schedule.set_progress(new_progress);
    }

    accept();


    accept();   // Accepted 상태로 닫기
}

void Schedule_Dialog::handle_delete_clicked()
{
    QMessageBox msg_box(this);
    msg_box.setWindowTitle("일정 삭제");
    msg_box.setText("정말로 이 일정을 삭제하시겠습니까?\n\""
                    + s_schedule.get_title() + "\"");
    msg_box.setIcon(QMessageBox::Question);

    // ⭐ 한글 버튼 직접 추가
    QPushButton *yes_button = msg_box.addButton("삭제", QMessageBox::YesRole);
    QPushButton *no_button = msg_box.addButton("취소", QMessageBox::NoRole);

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
        schedule_was_deleted = true;
        accept();
    }
}

bool Schedule_Dialog::is_deleted()
{
    return schedule_was_deleted;
}

Schedule Schedule_Dialog::get_updated_schedule()
{
    return s_schedule;
}

// 일정 생성 관련 함수
// 신규 모드: 기본값으로 초기화
void Schedule_Dialog::setup_new_mode_defaults(QString &default_project_name)
{
    // 프로젝트 콤보박스: 현재 보고 있는 프로젝트로 기본 선택
    int proj_index = ui->project_combobox->findText(default_project_name);
    if (proj_index >= 0) {
        ui->project_combobox->setCurrentIndex(proj_index);
    } else {
        // "모든 프로젝트" 상태였거나 매칭 실패 → 첫 번째 프로젝트
        ui->project_combobox->setCurrentIndex(0);
    }
    ui->project_combobox->setEnabled(true);  // 신규 모드에선 선택 가능

    ui->title_edit->setText("");
    ui->title_edit->setPlaceholderText("일정 제목을 입력하세요");
    ui->description_edit->setText("");
    ui->description_edit->setPlaceholderText("설명 (선택사항)");

    QDate today = QDate::currentDate();
    QDate one_week_later = today.addDays(7);
    ui->start_date_edit->setDate(today);
    ui->end_date_edit->setDate(one_week_later);

    ui->category_combobox->setCurrentIndex(0);  // work
}

// 생성 모드에서 main_page가 어느 프로젝트에 추가할지 알기 위함
QString Schedule_Dialog::get_selected_project_name()
{
    return ui->project_combobox->currentText();
}