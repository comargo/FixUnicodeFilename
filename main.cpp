#include "fixunicodefilenames.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FixUnicodeFilenames w;
    w.show();

    return a.exec();
}
