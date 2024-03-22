#pragma once

// STD
#include <iostream>
#include <format>
#include <cstdio>
#include <cstring>
#include <unistd.h>

// Imaging Source SDK
#include <gst/gst.h>
#include <tcam-property-1.0.h>

// Qt
#include <QObject>
#include <QTimer>

class ISCameraController : public QObject {
    Q_OBJECT
public:
    explicit ISCameraController(QObject* parent = nullptr);
    ~ISCameraController() override;

public slots:
    void OnModeChanged(int index);
    void OnCameraConnectionRequest();
    void OnCameraDisconnectionRequest();
    void OnCameraSelected(const int &cameraIndex);
    void OnGainChanged(int gain);
    void OnExposureTimeChanged(int exposureTime);
    void OnFocusImageCaptureRequest() const;
    void OnFocusImageAutoCaptureRequest(bool) const;

    signals:
    void CamerasFound(std::vector<std::pair<std::string, std::string>>& namesOfAvailableCameras) const;
    void CommunicationRequestHandled(bool) const;
    void GainReadFromHardware(int) const;
    void ExposureTimeReadFromHardware(int) const;
    void ImageCaptured(uint32_t*, int, int) const;

private:
    bool isCameraConnected;
    QString selectedCameraName;
    QString selectedCameraSerial;
    std::vector<std::pair<std::string, std::string>> listOfAvailableCameras;
    int gainInDB;
    int exposureTimeInMicroseconds;
    QTimer* autoCaptureTimer;

    int Nx;
    int Ny;
    uint32_t* imageBuffer;

    GstDeviceMonitor* monitor;
    GstElement* source;

private:
    void FindAvailableCameras();
    void Connect();
    void Disconnect();

private slots:
    void CaptureImage() const;
};
