// Qt6
#include <QApplication>
#include <memory>

// EPICS
#include <pva/client.h>

// Custom
#include "focusdiagnosticsmainwindow.h"
#include "Controller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto controller = new Controller();
    controller->ConnectSignalsAndSlots();
    controller->ShowView();

    return QApplication::exec();
}
