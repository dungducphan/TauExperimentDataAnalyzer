#pragma once

#include <gst/gst.h>
#include <tcam-property-1.0.h>

#include <QObject>

class ISCameraController : public QObject {
    Q_OBJECT
public:
    ISCameraController();
    ~ISCameraController() override;

    void FindAvailableCameras() const;

    signals:
    void CameraFound(std::vector<std::string> namesOfAvailableCameras) const;

};
