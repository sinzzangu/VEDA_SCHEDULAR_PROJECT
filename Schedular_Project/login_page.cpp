#include "login_page.h"
#include "login_register.h"
#include "ui_login_page.h"
#include <QDialog>
#include <QFrame>
#include <QPainter>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


}


Widget::~Widget()
{
    delete ui;
}



void Widget::on_pushButton_register_clicked()
{
    login_register register_page(this);
    register_page.exec();

}

