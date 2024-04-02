#include <RCController.h>

RCController::RCController(QObject *parent) :
QObject(parent),
mainWindow(new RCMainWindow()),
dataGrabber(new DataGrabber()) {
    ConnectSignalsAndSlots();
    ConnectToIOCs();
}

RCController::~RCController() {
    delete mainWindow;
}

void RCController::ShowView() {
    mainWindow->show();
}

void RCController::ConnectSignalsAndSlots() {

}

void RCController::ConnectToIOCs() {
    dataGrabber->ConnectToIOCs();
}
