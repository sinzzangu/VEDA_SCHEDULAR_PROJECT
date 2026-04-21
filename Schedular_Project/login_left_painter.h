#ifndef LOGIN_LEFT_PAINTER_H
#define LOGIN_LEFT_PAINTER_H
#include <QFrame>
#include <QPainter>
class login_left_painter : public QFrame
{
    Q_OBJECT
public:
    explicit login_left_painter(QFrame *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event) override;

signals:
};

#endif // LOGIN_LEFT_PAINTER_H
