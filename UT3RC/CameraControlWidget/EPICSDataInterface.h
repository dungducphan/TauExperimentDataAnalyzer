#pragma once

#include <iostream>

#include <QObject>
#include <CameraTypes.h>

class EPICSDataInterface : public QObject {
    Q_OBJECT
public:
    explicit EPICSDataInterface(QObject* parent = nullptr);
    ~EPICSDataInterface() override;

public slots:
    void CameraConnectRequested();
    void SelectedCameraChanged(int index);

private:
    CameraIndex_t selectedCamera;
};
