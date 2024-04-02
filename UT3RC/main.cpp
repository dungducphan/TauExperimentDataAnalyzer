#include <QApplication>

#include <RCController.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto rcController = new RCController();
    rcController->ShowView();

    return QApplication::exec();
}