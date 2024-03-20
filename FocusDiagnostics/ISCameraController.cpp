#include <ISCameraController.h>

ISCameraController::ISCameraController() = default;

ISCameraController::~ISCameraController() = default;

void ISCameraController::FindAvailableCameras() const {
    std::vector<std::string> namesOfAvailableCameras;

    // FIXME:
    // code to find available cameras here

    namesOfAvailableCameras.push_back("Camera 1");
    namesOfAvailableCameras.push_back("Camera 2");
    namesOfAvailableCameras.push_back("Camera 3");

    emit CameraFound(namesOfAvailableCameras);
}
