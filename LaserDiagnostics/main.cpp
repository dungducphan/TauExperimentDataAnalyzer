// Qt6
#include <QApplication>

// Custom
#include "CameraCoreGUI.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto cameraCoreGUI = new CameraCoreGUI();
    cameraCoreGUI->show();

    return QApplication::exec();
}
