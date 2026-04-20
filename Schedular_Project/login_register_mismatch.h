#ifndef LOGIN_REGISTER_MISMATCH_H
#define LOGIN_REGISTER_MISMATCH_H

#include <QDialog>

namespace Ui {
class login_register_mismatch;
}

class login_register_mismatch : public QDialog
{
    Q_OBJECT

public:
    explicit login_register_mismatch(QWidget *parent = nullptr);
    ~login_register_mismatch();

private slots:
    void on_mismatch_confirm_clicked();

private:
    Ui::login_register_mismatch *ui;
};

#endif // LOGIN_REGISTER_MISMATCH_H
