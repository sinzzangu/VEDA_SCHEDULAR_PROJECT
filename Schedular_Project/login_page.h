#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "ui_login_page.h"
#include <QWidget>
#include <QFrame>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void on_pushButton_register_clicked();

    void on_pushButton_login_clicked();
    QJsonObject get_obj_allUserData() {return obj_allUserData;}

private:
    Ui::Widget *ui;
    QJsonObject obj_allUserData;

    QString id;
    QString pw;

    void loadData(const QString &data);
    int login_check();
};



#endif // LOGIN_PAGE_H


