#include <ISCameraController.h>
#include <ISCameraHelpers.h>

ISCameraController::ISCameraController(QObject* parent) :
QObject(parent),
isCameraConnected(false),
selectedCameraName(""),
gainInDB(0),
exposureTimeInMicroseconds(0),
Nx(0),
Ny(0),
imageBuffer(nullptr),
monitor(nullptr),
source(nullptr),
pipeline_capture(nullptr),
autoCaptureEnabled(false) {
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
    GError* err = nullptr;
    pipeline_capture = gst_parse_launch("tcambin name=source ! videoconvert ! appsink name=sink", &err);
    source = gst_bin_get_by_name(GST_BIN(pipeline_capture), "source");
    const char* serial = selectedCameraSerial.toStdString().c_str();
    if (serial != nullptr) {
        GValue val = {};
        g_value_init(&val, G_TYPE_STRING);
        g_value_set_static_string(&val, serial);

        g_object_set_property(G_OBJECT(source), "serial", &val);
    }
    GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline_capture), "sink");
    g_object_set(G_OBJECT(sink), "emit-signals", TRUE, nullptr);
    g_signal_connect(sink, "new-sample", G_CALLBACK(this->CaptureImageCallback), this);
    gst_object_unref(sink);

    // in the READY state the camera will be initialized and properties will be available
    gst_element_set_state(source, GST_STATE_READY);
    gst_element_set_state(pipeline_capture, GST_STATE_READY);

    std::cout << selectedCameraName.toStdString() << " is connected!" << std::endl;
    isCameraConnected = true;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Once the connection is established, get the current gain and exposure time from the camera,
    // and set all other properties to default values
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    list_properties(source);
    double minGain, maxGain, minExposureTime, maxExposureTime;
    get_float_property_range(source, "Gain", minGain, maxGain);
    get_float_property_range(source, "ExposureTime", minExposureTime, maxExposureTime);
    FoundCameraGainRange((int) minGain, (int) maxGain);
    FoundCameraExposureTimeRange((int) minExposureTime, (int) maxExposureTime);

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
    // Stop the pipeline and frees all resources
    gst_element_set_state(pipeline_capture, GST_STATE_NULL);

    // Stop the pipeline and frees all resources
    gst_element_set_state(source, GST_STATE_NULL);
    gst_object_unref(source);

    // Pipeline automatically handles all elements that have been added to it.
    gst_object_unref(pipeline_capture);

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

    bool success = set_float_property(source, "Gain", gain);
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

    bool success = set_float_property(source, "ExposureTime", exposureTime);
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
    gst_element_set_state(pipeline_capture, GST_STATE_PLAYING);
}

void ISCameraController::OnFocusImageAutoCaptureRequest(bool autoCaptureEnabledArg) {
    if (!isCameraConnected) return;

    autoCaptureEnabled = autoCaptureEnabledArg;
    if (autoCaptureEnabled) {
        // Start capturing images
        gst_element_set_state(pipeline_capture, GST_STATE_PLAYING);
    } else {
        // Pause capturing images
        gst_element_set_state(pipeline_capture, GST_STATE_READY);
    }
}

GstFlowReturn ISCameraController::CaptureImageCallback(GstElement* sink, void* user_data) {
    auto cameraController = static_cast<ISCameraController*>(user_data);

    GstSample* sample = nullptr;
    /* Retrieve the buffer */
    g_signal_emit_by_name(sink, "pull-sample", &sample, nullptr);

    if (sample) { // Successfully captured an image
        static guint frameCount = 0;
        int pixel_data = -1;

        GstBuffer* buffer = gst_sample_get_buffer(sample);
        GstMapInfo info; // contains the actual image
        if (gst_buffer_map(buffer, &info, GST_MAP_READ)) {
            GstVideoInfo* video_info = gst_video_info_new();
            if (!gst_video_info_from_caps(video_info, gst_sample_get_caps(sample))) {
                // Could not parse video info (should not happen)
                g_warning("Failed to parse video info");
                return GST_FLOW_ERROR;
            }

            emit cameraController->ImageBeingProcessed();

            // Extract pixel data from the buffer
            cameraController->imageBuffer = new uint32_t[video_info->width * video_info->height];
            cameraController->Nx = video_info->width;
            cameraController->Ny = video_info->height;
            for (int i = 0; i < video_info->width * video_info->height; i++) {
                cameraController->imageBuffer[i] = (uint32_t) info.data[i];
            }

            gst_buffer_unmap(buffer, &info);
            gst_video_info_free(video_info);
        }

        GstClockTime timestamp = GST_BUFFER_PTS(buffer);
        g_print("Captured frame %d, Pixel Value=%03d Timestamp=%" GST_TIME_FORMAT "            \r",
                frameCount,
                pixel_data,
                GST_TIME_ARGS(timestamp));
        frameCount++;

        // delete our reference so that gstreamer can handle the sample
        gst_sample_unref(sample);
    }

    emit cameraController->ImageCaptured(cameraController->imageBuffer, cameraController->Nx, cameraController->Ny);

    return GST_FLOW_OK;
}

void ISCameraController::OnImageBeingProcessed() const {
    gst_element_set_state(pipeline_capture, GST_STATE_PAUSED);
}

void ISCameraController::OnImageProcessingCompleted() const {
    free(imageBuffer);
    if (autoCaptureEnabled) gst_element_set_state(pipeline_capture, GST_STATE_PLAYING);
    if (!autoCaptureEnabled) gst_element_set_state(pipeline_capture, GST_STATE_READY);
}
