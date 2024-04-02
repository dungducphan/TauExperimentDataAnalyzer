#pragma once

// STD LIBS
#include <iostream>
#include <thread>

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
    void OnCameraConnectRequested();
    void OnCameraDisconnectRequested();
    void OnSelectedCameraChanged(int index);

signals:
    void MonitorThreadStarted();
    void MonitorThreadFinished();
    void ImageReceived(const epics::pvData::shared_vector<const uint8_t>& image, const unsigned int& imageCounter);

private:
    void MonitorPV();

private:
    CameraIndex_t selectedCamera;
    pvac::ClientProvider* provider;
    pvac::ClientChannel* channel;
    pvac::MonitorSync* mon;
    unsigned int imageCounter;

    std::thread* monitorThread;
    bool isMonitoring;
};
