#include <ISCameraController.h>
#include <ISCameraHelpers.h>

ISCameraController::ISCameraController(QObject* parent) :
QObject(parent),
isCameraConnected(false),
selectedCameraName(""),
gainInDB(0),
exposureTimeInMicroseconds(0),
autoCaptureTimer(new QTimer(this)),
Nx(0),
Ny(0),
imageBuffer(nullptr),
monitor(nullptr),
source(nullptr) {
    connect(autoCaptureTimer, &QTimer::timeout, this, &ISCameraController::CaptureImage);
}

ISCameraController::~ISCameraController() = default;

void ISCameraController::FindAvailableCameras() {
    listOfAvailableCameras.clear();

    // The device monitor listens to device activities for us
    monitor = gst_device_monitor_new();

    // We are only interested in devices that are in the categories Video and Source and tcam
    gst_device_monitor_add_filter(monitor, "Video/Source/tcam", nullptr);

    // Static query: list all devices that are available right now
    GList* devices = gst_device_monitor_get_devices(monitor);
    for (GList* elem = devices; elem; elem = elem->next) {
        auto device = (GstDevice*) elem->data;
        GstStructure* metadataStruct = gst_device_get_properties(device);
        std::string deviceName = std::format("(S/N {}) - {}-{}", gst_structure_get_string(metadataStruct, "serial"), gst_structure_get_string(metadataStruct, "model"), gst_structure_get_string(metadataStruct, "type"));
        std::string deviceSerial = gst_structure_get_string(metadataStruct, "serial");
        listOfAvailableCameras.emplace_back(deviceName, deviceSerial);
        gst_structure_free(metadataStruct);
    }
    g_list_free_full(devices, gst_object_unref);

    emit CamerasFound(listOfAvailableCameras);
}

void ISCameraController::OnModeChanged(int index) {
    if (index != 0) return;
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
    /* create a tcambin to retrieve device information */
    source = gst_element_factory_make("tcambin", "source");
    const char* serial = selectedCameraSerial.toStdString().c_str();
    if (serial != nullptr) {
        GValue val = {};
        g_value_init(&val, G_TYPE_STRING);
        g_value_set_static_string(&val, serial);
        g_object_set_property(G_OBJECT(source), "serial", &val);
    }

    // in the READY state the camera will be initialized and properties will be available
    gst_element_set_state(source, GST_STATE_READY);

    std::cout << selectedCameraName.toStdString() << " is connected!" << std::endl;
    isCameraConnected = true;

    /////////////////////////////////////////////////////////////////////////////////////////////
    // Once the connection is established, get the current gain and exposure time from the camera,
    // and set all other properties to default values
    /////////////////////////////////////////////////////////////////////////////////////////////
    set_default_values(source);
    exposureTimeInMicroseconds = (int) get_float_property(source, "ExposureTime");
    gainInDB = (int) get_float_property(source, "Gain");

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
    gst_element_set_state(source, GST_STATE_NULL);
    gst_object_unref(source);
    std::cout << selectedCameraName.toStdString() << " is disconnected!" << std::endl;
    isCameraConnected = false;
}

void ISCameraController::OnCameraSelected(const int &cameraIndex) {
    selectedCameraName = listOfAvailableCameras[cameraIndex].first.c_str();
    selectedCameraSerial = listOfAvailableCameras[cameraIndex].second.c_str();
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
    emit ImageCaptured(imageBuffer, Nx, Ny);
}
