#pragma once

// Qt6 LIBS
#include <QObject>
#include <QTimer>

// JKQTPlotter LIBS
#include <jkqtplotter.h>

// CUSTOM LIBS
#include <CameraControl.h>
#include <ui_CameraControl.h>
#include <EPICSDataInterface.h>
#include <CameraTypes.h>

class CameraControl_Controller : public QObject {
    Q_OBJECT
public:
    explicit CameraControl_Controller(QWidget* parent = nullptr);
    ~CameraControl_Controller() override;

    void show();

public slots:
    void OnCameraConnectButtonClicked();
    void OnSelectedCameraChanged(int index);
    void OnCameraPVSubscriptionCompleted();

signals:
    void CameraConnectRequested();
    void SelectedCameraChanged(int index);

private:
    void Init();
    void ConnectSignalsAndSlots();

private:
    CameraControl* cameraControl;
    EPICSDataInterface* epicsDataInterface;
    CameraIndex_t selectedCamera;

    QTimer* timer;
};
