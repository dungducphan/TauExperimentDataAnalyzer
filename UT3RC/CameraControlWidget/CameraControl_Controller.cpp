#include <CameraControl_Controller.h>

CameraControl_Controller::CameraControl_Controller(QWidget *parent) :
QObject(parent),
cameraControl(new CameraControl(parent)),
epicsDataInterface(new EPICSDataInterface(parent)),
selectedCamera(CameraIndex_t::k_NOCAMERA),
timer(new QTimer(this)) {
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
    // Read the camera names from the CameraTypes.h file
    for (const auto& camera : CameraNames)
        cameraControl->GetUI()->comboBox_CAMERASELECT->addItem(camera.c_str());

    // Put Image Plotter into the UI layout
    cameraControl->GetImagePlotter()->getPlotter()->setUseAntiAliasingForGraphs(false);
    cameraControl->GetImagePlotter()->getPlotter()->setUseAntiAliasingForSystem(false);
    cameraControl->GetImagePlotter()->getPlotter()->setUseAntiAliasingForText(false);
    cameraControl->GetImagePlotter()->setGrid(false);
    cameraControl->GetImagePlotter()->setMousePositionShown(false);

    // QTimer for camera image update
    timer->setInterval(2000);
}

void CameraControl_Controller::ConnectSignalsAndSlots() {
    connect(cameraControl->GetUI()->comboBox_CAMERASELECT, &QComboBox::currentIndexChanged, this,
            &CameraControl_Controller::OnSelectedCameraChanged);
    connect(this, &CameraControl_Controller::SelectedCameraChanged, epicsDataInterface, &EPICSDataInterface::SelectedCameraChanged);

    connect(cameraControl->GetUI()->pushButton_CAMERACONNECT, &QPushButton::clicked, this, &CameraControl_Controller::OnCameraConnectButtonClicked);
    connect(this, &CameraControl_Controller::CameraConnectRequested, epicsDataInterface, &EPICSDataInterface::CameraConnectRequested);
    connect(epicsDataInterface, &EPICSDataInterface::CameraPVSubscriptionCompleted, this, &CameraControl_Controller::OnCameraPVSubscriptionCompleted);
    connect(timer, &QTimer::timeout, epicsDataInterface, &EPICSDataInterface::UpdateImage);
}

void CameraControl_Controller::OnCameraConnectButtonClicked() {
    if (selectedCamera == CameraIndex_t::k_NOCAMERA) {
        selectedCamera = static_cast<CameraIndex_t>(cameraControl->GetUI()->comboBox_CAMERASELECT->currentIndex());
        emit SelectedCameraChanged(selectedCamera);
    }
    emit CameraConnectRequested();
}

void CameraControl_Controller::OnSelectedCameraChanged(int index) {
    selectedCamera = static_cast<CameraIndex_t>(index);
    emit SelectedCameraChanged(selectedCamera);
}

void CameraControl_Controller::OnCameraPVSubscriptionCompleted() {
    timer->start();
}
