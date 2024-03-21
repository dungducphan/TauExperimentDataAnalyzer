// Qt6
#include <QApplication>
#include <QIcon>

// EPICS
#include <pva/client.h>

// Custom
#include <focusdiagnosticsmainwindow.h>
#include <Controller.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/Resources/Icons/icons8-laser-48.png"));

    auto controller = new Controller();
    controller->ShowView();

    return QApplication::exec();
}
