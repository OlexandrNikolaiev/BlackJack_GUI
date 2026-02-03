#include <QApplication>

#include "presentation/widgets/MainWindow/mainwindow.h"
#include "Infrastructure/Helpers/languagehelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LanguageHelper::instance().init();

    MainWindow w;
    w.show();

    return a.exec();
}
