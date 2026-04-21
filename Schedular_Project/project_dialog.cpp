/***********************
 * 04/21 MON
 * project_dialog.cpp
 * 프로젝트 관리 다이얼로그 구현
 * Created By 방준한
 * Version 2.0
 ***********************
 */
#include "project_dialog.h"
#include "ui_project_dialog.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

Project_Dialog::Project_Dialog(QList<Project> &projects, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Project_Dialog),
    s_projects(projects)
{
    ui->setupUi(this);
    // 일단 채워
    populate_list();

    // 버튼 연결
    connect(ui->add_button, SIGNAL(clicked()), this, SLOT(handle_add_clicked()));
    connect(ui->close_button, SIGNAL(clicked()), this, SLOT(accept()));
}

Project_Dialog::~Project_Dialog()
{
    delete ui;
}

// 전체 목록 다시 그리기
void Project_Dialog::populate_list()
{
    // 기존 리스트 지우고 안에 프로젝트 하나씩 채우기
    ui->project_list_widget->clear();
    for (int i = 0; i < s_projects.size(); i++) {
        add_project_row(s_projects[i]);
    }
}

// 프로젝트 하나를 QListWidget에 한 줄로 추가
void Project_Dialog::add_project_row(Project &project)
{
    // row widget 생성 (아이콘 + 이름 + 개수 + 삭제 버튼)
    QWidget *row_widget = new QWidget();
    QHBoxLayout *row_layout = new QHBoxLayout(row_widget);
    row_layout->setContentsMargins(12, 8, 12, 8);
    row_layout->setSpacing(10);

    // 폴더 아이콘 (이모지로 대체 — 간단)
    QLabel *icon_label = new QLabel("📁");
    icon_label->setFixedWidth(24);

    // 프로젝트 이름
    QLabel *name_label = new QLabel(project.get_name());
    name_label->setObjectName("row_name_label");

    // 일정 개수
    int count = project.get_schedule_count();
    QString count_text = QString::number(count) + "개 일정";
    QLabel *count_label = new QLabel(count_text);
    count_label->setObjectName("row_count_label");

    // 삭제 버튼
    QPushButton *delete_button = new QPushButton("삭제");
    delete_button->setObjectName("row_delete_button");
    delete_button->setCursor(Qt::PointingHandCursor);

    // 버튼에 project_id를 property로 저장 → 나중에 클릭 시 찾기 위함
    QString project_id = project.get_id();
    delete_button->setProperty("project_id", project_id);

    // connect 클릭 시 어떤 프로젝트인지 sender로 알아냄
    connect(delete_button, SIGNAL(clicked()), this, SLOT(handle_row_delete_clicked()));

    // 레이아웃에 한꺼번에 추가
    row_layout->addWidget(icon_label);
    row_layout->addWidget(name_label);
    row_layout->addStretch();
    row_layout->addWidget(count_label);
    row_layout->addWidget(delete_button);

    // QListWidgetItem 생성 후 widget 붙이기
    QListWidgetItem *item = new QListWidgetItem(ui->project_list_widget);
    item->setSizeHint(row_widget->sizeHint());
    ui->project_list_widget->setItemWidget(item, row_widget);
}

// + 추가 버튼 클릭
void Project_Dialog::handle_add_clicked()
{
    QString name = ui->project_name_edit->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "프로젝트 이름을 입력해주세요.");
        return;
    }

    // 중복 체크 (main_page도 하지만, 여기서도 먼저 체크해서 UX 개선)
    for (int i = 0; i < s_projects.size(); i++) {
        if (s_projects[i].get_name() == name) {
            QMessageBox::warning(this, "중복 오류",
                                 "이미 같은 이름의 프로젝트가 있습니다:\n\""
                                     + name + "\"");
            return;
        }
    }

    // 입력 필드 비우기
    ui->project_name_edit->clear();

    // main_page에게 추가 요청 → main_page가 처리 후 refresh_project_list 호출
    emit project_add_requested(name);
}

// 삭제 버튼 (각 row) 클릭
void Project_Dialog::handle_row_delete_clicked()
{
    // 어느 버튼이 눌렸는지 찾기
    QPushButton *clicked_button = (QPushButton*)sender();
    if (!clicked_button) return;

    QString target_id = clicked_button->property("project_id").toString();

    // projects에서 해당 id 찾기
    QString target_name;
    int target_count = 0;
    bool found = false;
    for (int i = 0; i < s_projects.size(); i++) {
        if (s_projects[i].get_id() == target_id) {
            target_name = s_projects[i].get_name();
            target_count = s_projects[i].get_schedule_count();
            found = true;
            break;
        }
    }

    if (!found) {
        qDebug() << "⚠️ 프로젝트 id를 찾을 수 없음:" << target_id;
        return;
    }

    // 확인 다이얼로그
    QMessageBox msg_box(this);
    msg_box.setWindowTitle("프로젝트 삭제");

    QString message;
    if (target_count > 0) {
        message = "\"" + target_name + "\" 프로젝트를 삭제하시겠습니까?\n\n"
                  + "이 프로젝트의 모든 일정 " + QString::number(target_count)
                  + "개가 함께 삭제됩니다.\n(이 작업은 되돌릴 수 없습니다)";
    } else {
        message = "\"" + target_name + "\" 프로젝트를 삭제하시겠습니까?";
    }
    msg_box.setText(message);
    msg_box.setIcon(QMessageBox::Warning);

    QPushButton *yes_button = msg_box.addButton("삭제", QMessageBox::YesRole);
    // QPushButton *no_button = msg_box.addButton("취소", QMessageBox::NoRole);

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

    if (msg_box.clickedButton() != yes_button) {
        return;
    }

    // main_page에게 삭제 요청
    emit project_delete_requested(target_id);
}

// main_page가 변경 후 호출 → 목록 다시 그림
void Project_Dialog::refresh_project_list(QList<Project> &projects)
{
    s_projects = projects;
    populate_list();
}