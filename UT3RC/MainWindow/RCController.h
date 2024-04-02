#pragma once

#include <QObject>

#include <RCMainWindow.h>
#include "UT3RC_autogen/include/ui_RCMainWindow.h"
#include <DataGrabber.h>

class RCController : public QObject {
    Q_OBJECT
public:
    explicit RCController(QObject* parent = nullptr);
    ~RCController() override;

    void ShowView();

private:
    RCMainWindow* mainWindow;
    DataGrabber* dataGrabber;

private:
    void ConnectSignalsAndSlots();
    void ConnectToIOCs();
};

