#ifndef LOGIN_REGISTER_SUCCESS_H
#define LOGIN_REGISTER_SUCCESS_H

#include <QDialog>

namespace Ui {
class login_register_success;
}

class login_register_success : public QDialog
{
    Q_OBJECT

public:
    explicit login_register_success(QWidget *parent = nullptr);
    ~login_register_success();

private slots:
    void on_mismatch_confirm_clicked();

private:
    Ui::login_register_success *ui;
};

#endif // LOGIN_REGISTER_SUCCESS_H
