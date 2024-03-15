#include <QApplication>
#include <focusdiagnosticsmainwindow.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    FocusDiagnosticsMainWindow w;
    w.show();

    return a.exec();
}
