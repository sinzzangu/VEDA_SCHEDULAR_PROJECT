#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "ui_login_page.h"
#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QPaintEvent> //test
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

private:


private:
    Ui::Widget *ui;

};



#endif // LOGIN_PAGE_H


