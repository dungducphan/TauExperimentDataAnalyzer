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
    delete dataGrabber;
}

void RCController::ShowView() {
    mainWindow->show();
}

void RCController::ConnectSignalsAndSlots() {
    connect(dataGrabber, &DataGrabber::ImageReceived, mainWindow, &RCMainWindow::OnImageReceived);
}

void RCController::ConnectToIOCs() {
    dataGrabber->ConnectToIOCs();
}
