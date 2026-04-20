#include "main_page.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // fusion for dark modes
    a.setStyle(QStyleFactory::create("Fusion"));
    main_page w;
    w.show();
    return QCoreApplication::exec();
}
