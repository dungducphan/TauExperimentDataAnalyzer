// Qt6
#include <QApplication>

// EPICS
#include <pva/client.h>

// Custom
#include "GUI/focusdiagnosticsmainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    FocusDiagnosticsMainWindow w;
    w.show();

    return QApplication::exec();
}
