/***********************
 * 04/21 MON
 * project_dialog.h
 * 프로젝트 관리 다이얼로그 (추가/삭제)
 * Created By 방준한
 * Version 2.0
 * 관리 센터 패턴으로 확장
 ***********************
 */
#ifndef PROJECT_DIALOG_H
#define PROJECT_DIALOG_H
#include <QDialog>
#include <QList>
#include "project.h"
namespace Ui {
class Project_Dialog;
}
class Project_Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Project_Dialog(QList<Project> &projects, QWidget *parent = nullptr);
    ~Project_Dialog();
signals:
    // 새 프로젝트 추가 요청 (main_page에서 처리 후 refresh 호출)
    void project_add_requested(QString name);
    // 프로젝트 삭제 요청 (main_page에서 처리 후 refresh 호출)
    void project_delete_requested(QString project_id);
public slots:
    // main_page가 projects 리스트를 업데이트한 후 호출 → 목록 다시 그림
    void refresh_project_list(QList<Project> &projects);
private slots:
    void handle_add_clicked();
    void handle_row_delete_clicked();
private:
    Ui::Project_Dialog *ui;
    QList<Project> s_projects;  // 현재 스냅샷 (refresh로 갱신됨)
    void populate_list();
    void add_project_row(Project &project);
};
#endif