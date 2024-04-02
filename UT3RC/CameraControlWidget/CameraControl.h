#pragma once

// Qt6 LIBS
#include <QWidget>

// JKQTPlotter LIBS
#include <jkqtplotter.h>
#include <graphs/jkqtpimage.h>

// EPICS LIBS
#include <epicsEvent.h>
#include <pva/client.h>

QT_BEGIN_NAMESPACE
namespace Ui { class CameraControl; }
QT_END_NAMESPACE

class CameraControl : public QWidget {
Q_OBJECT

public:
    explicit CameraControl(QWidget *parent = nullptr);
    ~CameraControl() override;

    Ui::CameraControl* GetUI() { return ui; }
    JKQTPlotter* GetImagePlotter() { return imagePlotter; }

public slots:
    void OnMonitorThreadStarted();
    void OnMonitorThreadFinished();
    void OnImageReceived(const epics::pvData::shared_vector<const uint8_t>& image, const unsigned int& imageCounter);


private:
    Ui::CameraControl *ui;
    JKQTPlotter* imagePlotter;
    JKQTPDatastore* ImageDataStore;
    size_t NPixelX;
    size_t NPixelY;
};
