#pragma once

// STD LIBS
#include <iostream>

// Qt6 LIBS
#include <QObject>

// CUSTOM LIBS
#include <CameraTypes.h>

// EPICS LIBS
#include <epicsEvent.h>
#include <pva/client.h>

class EPICSDataInterface : public QObject {
    Q_OBJECT
public:
    explicit EPICSDataInterface(QObject* parent = nullptr);
    ~EPICSDataInterface() override;

public slots:
    void CameraConnectRequested();
    void SelectedCameraChanged(int index);
    void UpdateImage();

signals:
    void CameraPVSubscriptionCompleted();

private:
    CameraIndex_t selectedCamera;
    pvac::ClientProvider* provider;
    pvac::ClientChannel* channel;
    pvac::MonitorSync* mon;
};
