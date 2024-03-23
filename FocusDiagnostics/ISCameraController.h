#pragma once

// STD
#include <iostream>
#include <format>
#include <cstdio>
#include <cstring>
#include <unistd.h>

// Imaging Source SDK
#include <gst/gst.h>
#include <gst/video/video.h>
#include <tcam-property-1.0.h>

// Qt
#include <QObject>

class ISCameraController : public QObject {
    Q_OBJECT
public:
    explicit ISCameraController(QObject* parent = nullptr);
    ~ISCameraController() override;

    uint32_t* imageBuffer;

public slots:
    void OnModeChanged(int index);
    void OnCameraConnectionRequest();
    void OnCameraDisconnectionRequest();
    void OnCameraSelected(const int &cameraIndex);
    void OnGainChanged(int gain);
    void OnExposureTimeChanged(int exposureTime);
    void OnFocusImageCaptureRequest() const;
    void OnFocusImageAutoCaptureRequest(bool);
    void OnImageBeingProcessed() const;
    void OnImageProcessingCompleted() const;
    static GstFlowReturn CaptureImageCallback(GstElement*, void*);

    signals:
    void CamerasFound(std::vector<std::pair<std::string, std::string>>& namesOfAvailableCameras) const;
    void CommunicationRequestHandled(bool) const;
    void GainReadFromHardware(int) const;
    void ExposureTimeReadFromHardware(int) const;
    void ImageCaptured(uint32_t*, int, int) const;
    void FoundCameraGainRange(int, int) const;
    void FoundCameraExposureTimeRange(int, int) const;
    void ImageBeingProcessed() const;

private:
    bool isCameraConnected;
    QString selectedCameraName;
    QString selectedCameraSerial;
    std::vector<std::pair<std::string, std::string>> listOfAvailableCameras;
    int gainInDB;
    int exposureTimeInMicroseconds;

    int Nx;
    int Ny;

    GstDeviceMonitor* monitor;
    GstElement* source;
    GstElement* pipeline_capture;
    bool autoCaptureEnabled;

private:
    void FindAvailableCameras();
    void Connect();
    void Disconnect();
};
