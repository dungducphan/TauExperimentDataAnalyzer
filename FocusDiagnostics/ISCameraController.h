#pragma once

// STD
#include <iostream>

// Image Capture SDK
#include <gst/gst.h>
#include <tcam-property-1.0.h>

// Qt
#include <QObject>

class ISCameraController : public QObject {
    Q_OBJECT
public:
    explicit ISCameraController(QObject* parent = nullptr);
    ~ISCameraController() override;

public slots:
    void OnModeChanged(int index) const;
    void OnCameraConnectionRequest();
    void OnCameraDisconnectionRequest();
    void OnCameraSelected(const QString& cameraName);

    signals:
    void CamerasFound(std::vector<std::string> namesOfAvailableCameras) const;
    void CommunicationRequestHandled(bool) const;
    void GainChanged(int) const;
    void ExposureTimeChanged(int) const;

private:
    bool isCameraConnected;
    QString selectedCameraName;
    int gainInDB;
    int exposureTimeInMicroseconds;

private:
    void FindAvailableCameras() const;
    void Connect();
    void Disconnect();
};
