/***********************
 * 04/21 MON
 * schedule_manager.cpp
 * 스케줄/프로젝트 데이터 관리 + JSON 영속성
 * Created By 방준한
 * Version 1.0
 ***********************
 */
#include "schedule_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDateTime>
#include <QDebug>

Schedule_Manager::Schedule_Manager(QObject *parent)
    : QObject(parent)
{
}

// JSON 파일에서 프로젝트 목록 로드
void Schedule_Manager::load_projects()
{
    // 바꿔야함
    QFile file(schedular_json_path);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file" << file.errorString();
        return;
    }

    QByteArray raw_data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(raw_data);

    if (doc.isNull()) {
        qWarning() << "JSON 파싱 실패:";
        return;
    }

    int projects_count = doc["projects"].toArray().size();

    for (int i = 0; i < projects_count; i++) {
        QString project_id = doc["projects"][i]["id"].toString();
        QString project_name = doc["projects"][i]["name"].toString();

        Project project(project_id, project_name);

        int schedule_count = doc["projects"][i]["schedules"].toArray().size();

        for (int j = 0; j < schedule_count; j++) {
            QJsonValue scheduleData = doc["projects"][i]["schedules"][j];
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

            project.add_schedule(schedule);
        }

        s_projects.append(project);
    }
}

// 프로젝트 목록을 JSON 파일로 저장
void Schedule_Manager::save_projects_to_json()
{
    QJsonObject root_object;
    QJsonArray projects_array;

    for (int i = 0; i < s_projects.size(); i++) {
        QJsonObject project_object;
        project_object["id"] = s_projects[i].get_id();
        project_object["name"] = s_projects[i].get_name();

        QJsonArray schedules_array;
        QList<Schedule> schedules = s_projects[i].get_schedules();

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

    root_object["projects"] = projects_array;

    QJsonDocument doc(root_object);

    QFile file(schedular_json_path);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "JSON 파일 쓰기 실패:";
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    // qDebug() << "JSON 저장 완료!";
}

// projects 리스트 참조 반환
QList<Project> &Schedule_Manager::get_projects()
{
    return s_projects;
}

// 프로젝트 갯수 반환
int Schedule_Manager::get_project_count()
{
    return s_projects.size();
}

// 특정 프로젝트에 스케줄 추가
void Schedule_Manager::add_schedule_to_project(QString &project_name, Schedule &new_schedule)
{
    for (int i = 0; i < s_projects.size(); i++) {
        if (s_projects[i].get_name() == project_name) {
            s_projects[i].add_schedule(new_schedule);
            qDebug() << "added project: " << project_name
                     << "schedule_title: " << new_schedule.get_title();
            return;
        }
    }
}

// 수정된 스케줄을 projects 리스트 안에서 교체
void Schedule_Manager::update_schedule_in_projects(Schedule &schedule)
{
    QString target_id = schedule.get_id();
    // 기존이랑 새로운거랑 id는 같으니까 그걸 기반으로 찾아서 수정
    for (int i = 0; i < s_projects.size(); i++) {
        QList<Schedule> schedules = s_projects[i].get_schedules();
        for (int j = 0; j < schedules.size(); j++) {
            if (schedules[j].get_id() == target_id) {
                s_projects[i].replace_schedule_by_id(target_id, schedule);
                qDebug() << "updated" << s_projects[i].get_name();
                return;
            }
        }
    }

    qDebug() << "cannot find schedule";
}

// id로 스케줄을 projects 리스트에서 삭제
void Schedule_Manager::delete_schedule_from_projects(QString id)
{
    // qDebug() << "trying to delete: " << id;

    for (int i = 0; i < s_projects.size(); i++) {
        s_projects[i].remove_schedule_by_id(id);
    }

    // qDebug() << "deleted";
}

// 새 프로젝트 추가
void Schedule_Manager::add_project(Project &new_project)
{
    s_projects.append(new_project);
    qDebug() << "project added: " << new_project.get_name();
}

// id로 프로젝트 삭제
void Schedule_Manager::delete_project_by_id(QString project_id)
{
    for (int i = 0; i < s_projects.size(); i++) {
        if (s_projects[i].get_id() == project_id) {
            QString deleted_name = s_projects[i].get_name();
            s_projects.removeAt(i);
            qDebug() << "project_deleted: " << deleted_name;
            return;
        }
    }
    qDebug() << "can't find " << project_id;
}

// 스케줄 id 생성
QString Schedule_Manager::generate_schedule_id()
{
    int timestamp_ms = QDateTime::currentMSecsSinceEpoch();
    QString id = "sch_" + QString::number(timestamp_ms);
    return id;
}

// 프로젝트 id 생성
QString Schedule_Manager::generate_project_id()
{
    int timestamp_ms = QDateTime::currentMSecsSinceEpoch();
    QString id = "proj_" + QString::number(timestamp_ms);
    return id;
}

// 프로젝트 이름 중복 체크
bool Schedule_Manager::is_project_name_duplicate(QString &name)
{
    for (int i = 0; i < s_projects.size(); i++) {
        if (s_projects[i].get_name() == name) {
            return true;
        }
    }
    return false;
}