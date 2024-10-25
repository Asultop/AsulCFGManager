#include "FluentHiper.h"

#include <QApplication>
#include <ElaApplication.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FluentHiper w;

    eApp->init();
    w.show();
    return a.exec();
}
