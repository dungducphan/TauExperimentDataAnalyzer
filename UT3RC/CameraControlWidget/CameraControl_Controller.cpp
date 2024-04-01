#include <CameraControl_Controller.h>

CameraControl_Controller::CameraControl_Controller(QWidget *parent) :
QObject(parent),
cameraControl(new CameraControl(parent)),
epicsDataInterface(new EPICSDataInterface(parent)),
selectedCamera(CameraIndex_t::k_NOCAMERA) {
    Init();
    ConnectSignalsAndSlots();
}

CameraControl_Controller::~CameraControl_Controller() {
    delete cameraControl;
    delete epicsDataInterface;
}

void CameraControl_Controller::show() {
    cameraControl->show();
}

void CameraControl_Controller::Init() {
    for (const auto& camera : CameraNames)
        cameraControl->GetUI()->comboBox_CAMERASELECT->addItem(camera.c_str());
}

void CameraControl_Controller::ConnectSignalsAndSlots() {
    connect(cameraControl->GetUI()->comboBox_CAMERASELECT, &QComboBox::currentIndexChanged, this, &CameraControl_Controller::SelectedCameraChanged);
    connect(this, &CameraControl_Controller::OnSelectedCameraChanged, epicsDataInterface, &EPICSDataInterface::SelectedCameraChanged);

    connect(cameraControl->GetUI()->pushButton_CAMERACONNECT, &QPushButton::clicked, this, &CameraControl_Controller::CameraConnectRequested);
    connect(this, &CameraControl_Controller::OnCameraConnectRequested, epicsDataInterface, &EPICSDataInterface::CameraConnectRequested);
}

void CameraControl_Controller::CameraConnectRequested() {
    if (selectedCamera == CameraIndex_t::k_NOCAMERA) {
        selectedCamera = static_cast<CameraIndex_t>(cameraControl->GetUI()->comboBox_CAMERASELECT->currentIndex());
        emit OnSelectedCameraChanged(selectedCamera);
    }
    emit OnCameraConnectRequested();
}

void CameraControl_Controller::SelectedCameraChanged(int index) {
    selectedCamera = static_cast<CameraIndex_t>(index);
    emit OnSelectedCameraChanged(selectedCamera);
}
