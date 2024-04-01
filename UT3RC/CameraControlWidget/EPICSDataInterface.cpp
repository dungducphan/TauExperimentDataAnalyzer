#include <EPICSDataInterface.h>

EPICSDataInterface::EPICSDataInterface(QObject *parent) :
QObject(parent),
selectedCamera(CameraIndex_t::k_NOCAMERA) {

}

EPICSDataInterface::~EPICSDataInterface() {

}

void EPICSDataInterface::CameraConnectRequested() {
    std::cout << "Connect Camera: " << ImageSources[selectedCamera] << std::endl;
}

void EPICSDataInterface::SelectedCameraChanged(int index) {
    selectedCamera = static_cast<CameraIndex_t>(index);
    std::cout << "Selected Camera: " << ImageSources[selectedCamera] << std::endl;
}


