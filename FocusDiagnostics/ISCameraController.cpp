#include <ISCameraController.h>

ISCameraController::ISCameraController(QObject* parent) :
QObject(parent),
isCameraConnected(false),
selectedCameraName(""),
gainInDB(0),
exposureTimeInMicroseconds(0),
autoCaptureTimer(new QTimer(this)) {
    connect(autoCaptureTimer, &QTimer::timeout, this, &ISCameraController::CaptureImage);
}

ISCameraController::~ISCameraController() = default;

void ISCameraController::FindAvailableCameras() const {
    std::vector<std::string> namesOfAvailableCameras;

    // FIXME:
    // code to find available cameras here
    // replace the following line with the actual codes
    namesOfAvailableCameras.emplace_back("Camera 1");
    namesOfAvailableCameras.emplace_back("Camera 2");
    namesOfAvailableCameras.emplace_back("Camera 3");

    emit CamerasFound(namesOfAvailableCameras);
}

void ISCameraController::OnModeChanged(int index) const {
    if (index != 1) return;
    if (!isCameraConnected) FindAvailableCameras();
}

void ISCameraController::OnCameraConnectionRequest() {
    if (isCameraConnected) {
        std::cout << "Camera is already connected. I don't know why this is happening!" << std::endl;
    } else {
        Connect();
    }

    emit CommunicationRequestHandled(isCameraConnected);
}

void ISCameraController::Connect() {
    // FIXME
    // code to establish connection with the camera here

    std::cout << selectedCameraName.toStdString() << " is connected!" << std::endl;
    isCameraConnected = true;
    // once the connection is established, get the current gain and exposure time from the camera

    // FIXME
    // This is just a mock code to simulate gain and exposure time control
    if (selectedCameraName == "Camera 1") {
        gainInDB = 10;
        exposureTimeInMicroseconds = 100000;
    } else if (selectedCameraName == "Camera 2") {
        gainInDB = 20;
        exposureTimeInMicroseconds = 200000;
    } else if (selectedCameraName == "Camera 3") {
        gainInDB = 30;
        exposureTimeInMicroseconds = 300000;
    }

    emit GainReadFromHardware(gainInDB);
    emit ExposureTimeReadFromHardware(exposureTimeInMicroseconds);
}

void ISCameraController::OnCameraDisconnectionRequest() {
    if (!isCameraConnected) {
        std::cout << "Camera is already disconnected. I don't know why this is happening!" << std::endl;
    } else {
        Disconnect();
    }

    emit CommunicationRequestHandled(isCameraConnected);
}

void ISCameraController::Disconnect() {
    // FIXME
    // code to disconnect with the camera here
    std::cout << selectedCameraName.toStdString() << " is disconnected!" << std::endl;
    isCameraConnected = false;
}

void ISCameraController::OnCameraSelected(const QString &cameraName) {
    selectedCameraName = cameraName;
}

void ISCameraController::OnGainChanged(int gain) {
    if (!isCameraConnected) return;
    if (gainInDB == gain) return;

    // FIXME
    // code to set the gain of the camera here, using the gain from the argument
    std::cout << "Gain is set to " << gain << " dB" << std::endl;
    bool success = true; // replace this line with the actual code to set the gain

    if (success) {
        // if above piece of code succeeds, update the gainInDB
        gainInDB = gain;
    } else {
        // if the above piece of code DOES NOT succeed, rollback the previous value
        emit ExposureTimeReadFromHardware(gainInDB);
    }
}

void ISCameraController::OnExposureTimeChanged(int exposureTime) {
    if (!isCameraConnected) return;
    if (exposureTimeInMicroseconds == exposureTime) return;

    // FIXME
    // code to set the exposure time of the camera here, using the exposure time from the argument
    std::cout << "Exposure time is set to " << exposureTime << " microseconds" << std::endl;
    bool success = true; // replace this line with the actual code to set the exposure time

    if (success) {
        // if above piece of code succeeds, update the exposureTimeInMicroseconds
        exposureTimeInMicroseconds = exposureTime;
    } else {
        // if the above piece of code DOES NOT succeed, rollback the previous value
        emit ExposureTimeReadFromHardware(exposureTimeInMicroseconds);
    }
}

void ISCameraController::OnFocusImageCaptureRequest() const {
    if (!isCameraConnected) return;
    CaptureImage();
}

void ISCameraController::OnFocusImageAutoCaptureRequest(bool autoCaptureEnabled) const {
    if (!isCameraConnected) return;
    autoCaptureEnabled ? autoCaptureTimer->start(1000) : autoCaptureTimer->stop();
}

void ISCameraController::CaptureImage() const {
    // FIXME
    // code to capture an image from the camera here
    std::cout << "Image is captured!" << std::endl;
}
