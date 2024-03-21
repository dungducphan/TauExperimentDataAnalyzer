#include "Controller.h"

Controller::Controller(QObject* parent) :
        QObject(parent),
        displayMode(1),
        isAcquiring(false),
        gainInDecibels(0),
        exposureTimeInMicroseconds(0),
        beamEnergyInMilliJoules(0),
        pulseDurationInFemtoSeconds(0),
        beamFWHMX(0),
        beamFWHMY(0),
        normalizedVectorPotential(0),
        cameraController(new ISCameraController(this)),
        isCameraConnected(false),
        autoCaptureEnabled(false) {
    view = new FocusDiagnosticsMainWindow();
    imageDataModel = new ImageDataModel(this);
    ConnectSignalsAndSlots();
    Initialize();
}

void Controller::Initialize() {
    // DISPLAY MODE
    view->ui->combobox_MODE->setCurrentIndex(displayMode);

    // FOCUS IMAGE TAB LAYOUT
    imageDataModel->GetFocusImagePlot()->getPlotter()->setUseAntiAliasingForGraphs(true);
    imageDataModel->GetFocusImagePlot()->getPlotter()->setUseAntiAliasingForSystem(true);
    imageDataModel->GetFocusImagePlot()->getPlotter()->setUseAntiAliasingForText(true);
    imageDataModel->GetFocusImagePlot()->setGrid(false);
    imageDataModel->GetFocusImagePlot()->setMousePositionShown(false);

    imageDataModel->GetFocusImagePlot_ProjectionX()->getPlotter()->setUseAntiAliasingForGraphs(true);
    imageDataModel->GetFocusImagePlot_ProjectionX()->getPlotter()->setUseAntiAliasingForSystem(true);
    imageDataModel->GetFocusImagePlot_ProjectionX()->getPlotter()->setUseAntiAliasingForText(true);
    imageDataModel->GetFocusImagePlot_ProjectionX()->setGrid(false);
    imageDataModel->GetFocusImagePlot_ProjectionX()->setMousePositionShown(false);
    imageDataModel->GetFocusImagePlot_ProjectionX()->synchronizeXToMaster(imageDataModel->GetFocusImagePlot());
    imageDataModel->GetFocusImagePlot_ProjectionX()->getXAxis()->setDrawMode1(JKQTPCADMLineTicks);
    imageDataModel->GetFocusImagePlot_ProjectionX()->getYAxis()->setDrawMode1(JKQTPCADMLineTicks);
    imageDataModel->GetFocusImagePlot_ProjectionX()->getXAxis()->setShowZeroAxis(false);
    imageDataModel->GetFocusImagePlot_ProjectionX()->getYAxis()->setShowZeroAxis(false);

    imageDataModel->GetFocusImagePlot_ProjectionY()->getPlotter()->setUseAntiAliasingForGraphs(true);
    imageDataModel->GetFocusImagePlot_ProjectionY()->getPlotter()->setUseAntiAliasingForSystem(true);
    imageDataModel->GetFocusImagePlot_ProjectionY()->getPlotter()->setUseAntiAliasingForText(true);
    imageDataModel->GetFocusImagePlot_ProjectionY()->setGrid(false);
    imageDataModel->GetFocusImagePlot_ProjectionY()->setMousePositionShown(false);
    imageDataModel->GetFocusImagePlot_ProjectionY()->synchronizeYToMaster(imageDataModel->GetFocusImagePlot());
    imageDataModel->GetFocusImagePlot_ProjectionY()->getXAxis()->setDrawMode1(JKQTPCADMLineTicks);
    imageDataModel->GetFocusImagePlot_ProjectionY()->getYAxis()->setDrawMode1(JKQTPCADMLineTicks);
    imageDataModel->GetFocusImagePlot_ProjectionY()->getXAxis()->setShowZeroAxis(false);
    imageDataModel->GetFocusImagePlot_ProjectionY()->getYAxis()->setShowZeroAxis(false);

    view->ui->widget_IMAGE->setLayout(new QGridLayout);
    imageDataModel->GetFocusImagePlot()->setParent(view->ui->widget_IMAGE);
    imageDataModel->GetFocusImagePlot_ProjectionX()->setParent(view->ui->widget_IMAGE);
    imageDataModel->GetFocusImagePlot_ProjectionY()->setParent(view->ui->widget_IMAGE);
    auto layout = dynamic_cast<QGridLayout*>(view->ui->widget_IMAGE->layout());
    layout->addWidget(imageDataModel->GetFocusImagePlot(), 0, 1);
    layout->addWidget(imageDataModel->GetFocusImagePlot_ProjectionY(), 0, 0);
    layout->addWidget(imageDataModel->GetFocusImagePlot_ProjectionX(), 1, 1);
    layout->setColumnStretch(0, 2);
    layout->setColumnStretch(1, 7);
    layout->setRowStretch(0, 7);
    layout->setRowStretch(1, 2);

    // Beam Energy
    beamEnergyInMilliJoules = view->ui->spinbox_BEAM_ENERGY->value();
    imageDataModel->SetBeamEnergy(beamEnergyInMilliJoules);

    // Pulse Duration
    pulseDurationInFemtoSeconds = view->ui->spinbox_PULSE_DURATION->value();
    imageDataModel->SetPulseDuration(pulseDurationInFemtoSeconds);
}

Controller::~Controller() {
    delete view;
    delete imageDataModel;
}

void Controller::ShowView() {
    view->show();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SIGNALS AND SLOTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Controller::ConnectSignalsAndSlots() {
    // GAIN
    connect(view->ui->slider_GAIN, &QSlider::valueChanged, this, &Controller::OnGainChangedFromSlider);
    connect(view->ui->spinbox_GAIN, &QSpinBox::editingFinished, this, &Controller::OnGainChangedFromSpinBox);
    connect(cameraController, &ISCameraController::GainReadFromHardware, this, &Controller::OnGainReadFromHardware);
    connect(this, &Controller::GainChanged, view, &FocusDiagnosticsMainWindow::OnGainChanged);
    connect(this, &Controller::GainChanged, cameraController, &ISCameraController::OnGainChanged);

    // EXPOSURE TIME
    connect(view->ui->slider_EXPOSURE_TIME, &QSlider::valueChanged, this, &Controller::OnExposureTimeChangedFromSlider);
    connect(view->ui->spinbox_EXPOSURE_TIME, &QSpinBox::editingFinished, this, &Controller::OnExposureTimeChangedFromSpinBox);
    connect(cameraController, &ISCameraController::ExposureTimeReadFromHardware, this, &Controller::OnExposureTimeReadFromHardware);
    connect(this, &Controller::ExposureTimeChanged, view, &FocusDiagnosticsMainWindow::OnExposureTimeChanged);
    connect(this, &Controller::ExposureTimeChanged, cameraController, &ISCameraController::OnExposureTimeChanged);

    // BEAM ENERGY
    connect(view->ui->spinbox_BEAM_ENERGY, &QSpinBox::editingFinished, this, &Controller::OnBeamEnergyChanged);
    connect(this, &Controller::BeamEnergyChanged, view, &FocusDiagnosticsMainWindow::OnBeamEnergyChanged);
    connect(this, &Controller::BeamEnergyChanged, imageDataModel, &ImageDataModel::OnBeamEnergyChanged);

    // PULSE DURATION
    connect(view->ui->spinbox_PULSE_DURATION, &QSpinBox::editingFinished, this, &Controller::OnPulseDurationChanged);
    connect(this, &Controller::PulseDurationChanged, view, &FocusDiagnosticsMainWindow::OnPulseDurationChanged);
    connect(this, &Controller::PulseDurationChanged, imageDataModel, &ImageDataModel::OnPulseDurationChanged);

    // DISPLAY MODE
    connect(view->ui->combobox_MODE, &QComboBox::currentIndexChanged, this, &Controller::OnModeChanged);
    connect(this, &Controller::ModeChanged, view, &FocusDiagnosticsMainWindow::OnModeChanged);
    connect(this, &Controller::ModeChanged, imageDataModel, &ImageDataModel::OnModeChanged);
    connect(this, &Controller::ModeChanged, cameraController, &ISCameraController::OnModeChanged);

    // SEARCHING CAMERAS
    connect(cameraController, &ISCameraController::CamerasFound, this, &Controller::OnCamerasFound);
    connect(this, &Controller::CamerasFound, view, &FocusDiagnosticsMainWindow::OnCamerasFound);

    // SELECT CAMERA
    connect(view->ui->comboBox_CAMERA_LIST, &QComboBox::currentTextChanged, this, &Controller::OnCameraSelected);
    connect(this, &Controller::CameraSelected, cameraController, &ISCameraController::OnCameraSelected);

    // CLICK CONNECT BUTTON ON SELECTED CAMERA
    connect(view->ui->button_CAMERA_CONNECTION, &QPushButton::clicked, this, &Controller::OnCameraConnectionButtonClicked);
    connect(this, &Controller::CameraConnectionRequest, cameraController, &ISCameraController::OnCameraConnectionRequest);
    connect(this, &Controller::CameraDisconnectionRequest, cameraController, &ISCameraController::OnCameraDisconnectionRequest);

    // CAMERA CONNECTION STATUS
    connect(cameraController, &ISCameraController::CommunicationRequestHandled, this, &Controller::OnCommunicationRequestHandled);
    connect(this, &Controller::CommunicationRequestHandled, view, &FocusDiagnosticsMainWindow::OnCommunicationRequestHandled);

    // FOCUS IMAGE FILE SELECT
    connect(view->ui->button_FOCUS_IMAGE_FILE_SELECT, &QPushButton::clicked, this, &Controller::OnFocusImageFileSelectButtonClicked);
    connect(this, &Controller::FocusImageFileSelected, imageDataModel, &ImageDataModel::OnFocusImageFileSelected);
    connect(this, &Controller::FocusImageFileSelected, view, &FocusDiagnosticsMainWindow::OnFocusImageFileSelected);

    // BEAM FWHM CALCULATED
    connect(imageDataModel, &ImageDataModel::BeamFWHMCalculated, this, &Controller::OnBeamFWHMCalculated);
    connect(this, &Controller::BeamFWHMCalculated, view, &FocusDiagnosticsMainWindow::OnBeamFWHMCalculated);

    // NORMALIZED VECTOR POTENTIAL CALCULATED
    connect(imageDataModel, &ImageDataModel::NormalizedVectorPotentialCalculated, this, &Controller::OnNormalizedVectorPotentialCalculated);
    connect(this, &Controller::NormalizedVectorPotentialCalculated, view, &FocusDiagnosticsMainWindow::OnNormalizedVectorPotentialCalculated);

    // ACQUIRE BUTTON
    connect(view->ui->button_FOCUS_IMAGE_CAPTURE, &QPushButton::clicked, this, &Controller::OnSingleAcquisitionButtonClicked);
    connect(view->ui->button_FOCUS_IMAGE_AUTO_CAPTURE, &QPushButton::clicked, this, &Controller::OnAutoAcquisitionButtonClicked);
    connect(this, &Controller::FocusImageCaptureRequest, cameraController, &ISCameraController::OnFocusImageCaptureRequest);
    connect(this, &Controller::FocusImageAutoCaptureRequest, cameraController, &ISCameraController::OnFocusImageAutoCaptureRequest);
}

void Controller::OnGainChangedFromSlider(int gain) {
    gainInDecibels = gain;
    emit GainChanged(gainInDecibels);
}

void Controller::OnGainChangedFromSpinBox() {
    gainInDecibels = view->ui->spinbox_GAIN->value();;
    emit GainChanged(gainInDecibels);
}

void Controller::OnGainReadFromHardware(int gain) {
    gainInDecibels = gain;
    emit GainChanged(gainInDecibels);
}

void Controller::OnExposureTimeChangedFromSlider(int exposureTime) {
    exposureTimeInMicroseconds = exposureTime;
    emit ExposureTimeChanged(exposureTimeInMicroseconds);
}

void Controller::OnExposureTimeChangedFromSpinBox() {
    exposureTimeInMicroseconds = view->ui->spinbox_EXPOSURE_TIME->value();
    emit ExposureTimeChanged(exposureTimeInMicroseconds);
}

void Controller::OnExposureTimeReadFromHardware(int exposureTime) {
    exposureTimeInMicroseconds = exposureTime;
    emit ExposureTimeChanged(exposureTimeInMicroseconds);
}

void Controller::OnBeamEnergyChanged() {
    beamEnergyInMilliJoules = view->ui->spinbox_BEAM_ENERGY->value();
    emit BeamEnergyChanged(beamEnergyInMilliJoules);
}

void Controller::OnPulseDurationChanged() {
    pulseDurationInFemtoSeconds = view->ui->spinbox_PULSE_DURATION->value();
    emit PulseDurationChanged(pulseDurationInFemtoSeconds);
}

void Controller::OnModeChanged(int index) {
    displayMode = index;
    emit ModeChanged(displayMode);
}

void Controller::OnSingleAcquisitionButtonClicked() const {
    if (!isCameraConnected) return;
    emit FocusImageCaptureRequest();
}

void Controller::DisableCameraControls() {
    view->ui->button_CAMERA_CONNECTION->setDisabled(true);
    view->ui->comboBox_CAMERA_LIST->setDisabled(true);
    view->ui->slider_GAIN->setDisabled(true);
    view->ui->spinbox_GAIN->setDisabled(true);
    view->ui->slider_EXPOSURE_TIME->setDisabled(true);
    view->ui->spinbox_EXPOSURE_TIME->setDisabled(true);
}

void Controller::EnableCameraControls() {
    view->ui->button_CAMERA_CONNECTION->setDisabled(false);
    view->ui->comboBox_CAMERA_LIST->setDisabled(false);
    view->ui->slider_GAIN->setDisabled(false);
    view->ui->spinbox_GAIN->setDisabled(false);
    view->ui->slider_EXPOSURE_TIME->setDisabled(false);
    view->ui->spinbox_EXPOSURE_TIME->setDisabled(false);
}

void Controller::OnAutoAcquisitionButtonClicked() {
    if (!isCameraConnected) return;

    if (view->ui->button_FOCUS_IMAGE_AUTO_CAPTURE->text() == "Auto") {
        autoCaptureEnabled = true;
        view->ui->button_FOCUS_IMAGE_AUTO_CAPTURE->setText("Stop");
        DisableCameraControls();
        emit FocusImageAutoCaptureRequest(autoCaptureEnabled);
    } else {
        autoCaptureEnabled = false;
        view->ui->button_FOCUS_IMAGE_AUTO_CAPTURE->setText("Auto");
        EnableCameraControls();
        emit FocusImageAutoCaptureRequest(autoCaptureEnabled);
    }
}

void Controller::OnFocusImageFileSelectButtonClicked() {
    auto filename = QFileDialog::getOpenFileName(view, "Select Focus Image", QDir::homePath(), "Image Files (*.tiff *.tif)");
    emit FocusImageFileSelected(filename);
}

void Controller::OnBeamFWHMCalculated(double FWHMX, double FWHMY) {
    beamFWHMX = FWHMX;
    beamFWHMY = FWHMY;
    emit BeamFWHMCalculated(beamFWHMX, beamFWHMY);
}

void Controller::OnNormalizedVectorPotentialCalculated(double A0) {
    normalizedVectorPotential = A0;
    emit NormalizedVectorPotentialCalculated(normalizedVectorPotential);
}

void Controller::OnCamerasFound(const std::vector<std::string> &availableCameras) {
    namesOfAvailableCameras.clear();
    for (auto& elem : availableCameras) namesOfAvailableCameras.push_back(elem);
    emit CamerasFound(namesOfAvailableCameras);
}

void Controller::OnCameraConnectionButtonClicked() const {
    std::cout << "Button clicked\n";
    std::cout << "isCameraConnected: " << isCameraConnected << std::endl;
    if (!isCameraConnected) {
        emit CameraConnectionRequest();
    } else {
        emit CameraDisconnectionRequest();
    }
}

void Controller::OnCommunicationRequestHandled(bool camConnect) {
    isCameraConnected = camConnect;
    emit CommunicationRequestHandled(isCameraConnected);
}

void Controller::OnCameraSelected(const QString &cameraName) {
    selectedCameraName = cameraName;
    emit CameraSelected(selectedCameraName);
}
