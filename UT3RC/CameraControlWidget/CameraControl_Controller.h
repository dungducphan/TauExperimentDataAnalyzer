#pragma once

#include <QObject>

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
    void CameraConnectRequested();
    void SelectedCameraChanged(int index);

signals:
    void OnCameraConnectRequested();
    void OnSelectedCameraChanged(int index);

private:
    void Init();
    void ConnectSignalsAndSlots();

private:
    CameraControl* cameraControl;
    EPICSDataInterface* epicsDataInterface;
    CameraIndex_t selectedCamera;
};
