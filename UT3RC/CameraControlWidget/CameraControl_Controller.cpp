#include <CameraControl_Controller.h>

CameraControl_Controller::CameraControl_Controller(QWidget *parent) :
QObject(parent),
cameraControl(new CameraControl(parent)),
epicsDataInterface(new EPICSDataInterface(parent)),
selectedCamera(CameraIndex_t::k_NOCAMERA),
isMonitoring(false) {
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

    // Initial state of the UI
    if (isMonitoring) {
        cameraControl->GetUI()->pushButton_CAMERACONNECT->setText("Disconnect");
        cameraControl->GetUI()->pushButton_CAMERACONNECT->setStyleSheet("background-color: red");
    } else {
        cameraControl->GetUI()->pushButton_CAMERACONNECT->setText("Connect");
        cameraControl->GetUI()->pushButton_CAMERACONNECT->setStyleSheet("background-color: green");
    }
}

void CameraControl_Controller::ConnectSignalsAndSlots() {
    connect(cameraControl->GetUI()->comboBox_CAMERASELECT, &QComboBox::currentIndexChanged, this,
            &CameraControl_Controller::OnSelectedCameraChanged);
    connect(this, &CameraControl_Controller::SelectedCameraChanged, epicsDataInterface,
            &EPICSDataInterface::OnSelectedCameraChanged);

    connect(cameraControl->GetUI()->pushButton_CAMERACONNECT, &QPushButton::clicked, this, &CameraControl_Controller::OnCameraConnectButtonClicked);
    connect(this, &CameraControl_Controller::CameraConnectRequested, epicsDataInterface,
            &EPICSDataInterface::OnCameraConnectRequested);
    connect(this, &CameraControl_Controller::CameraDisconnectRequested, epicsDataInterface,
            &EPICSDataInterface::OnCameraDisconnectRequested);
    connect(epicsDataInterface, &EPICSDataInterface::MonitorThreadStarted, this,
            &CameraControl_Controller::OnMonitorThreadStarted);
    connect(epicsDataInterface, &EPICSDataInterface::MonitorThreadFinished, this,
            &CameraControl_Controller::OnMonitorThreadFinished);
    connect(this, &CameraControl_Controller::UpdateUIOnMonitorThreadStarted, cameraControl, &CameraControl::OnMonitorThreadStarted);
    connect(this, &CameraControl_Controller::UpdateUIOnMonitorThreadFinished, cameraControl, &CameraControl::OnMonitorThreadFinished);

    connect(epicsDataInterface, &EPICSDataInterface::ImageReceived, cameraControl, &CameraControl::OnImageReceived);
}

void CameraControl_Controller::OnCameraConnectButtonClicked() {
    if (selectedCamera == CameraIndex_t::k_NOCAMERA) {
        selectedCamera = static_cast<CameraIndex_t>(cameraControl->GetUI()->comboBox_CAMERASELECT->currentIndex());
        emit SelectedCameraChanged(selectedCamera);
    }
    if (selectedCamera != CameraIndex_t::k_NOCAMERA) {
        isMonitoring ? emit CameraDisconnectRequested() : emit CameraConnectRequested();
    }
}

void CameraControl_Controller::OnSelectedCameraChanged(int index) {
    selectedCamera = static_cast<CameraIndex_t>(index);
    emit SelectedCameraChanged(selectedCamera);
}

void CameraControl_Controller::OnMonitorThreadStarted() {
    isMonitoring = true;
    emit UpdateUIOnMonitorThreadStarted();
}

void CameraControl_Controller::OnMonitorThreadFinished() {
    isMonitoring = false;
    emit UpdateUIOnMonitorThreadFinished();
}
