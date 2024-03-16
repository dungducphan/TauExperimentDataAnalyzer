#include "Controller.h"

Controller::Controller() :
        QObject(nullptr),
        displayMode(0),
        isAcquiring(false),
        gainInDecibels(0),
        exposureTimeInMicroseconds(0),
        beamEnergyInMilliJoules(0) {
    view = new FocusDiagnosticsMainWindow();
    imageDataModel = new ImageDataModel(this);
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
    imageDataModel->GetFocusImagePlot()->getXAxis()->setDrawMode1(JKQTPCADMLineTicks);
    imageDataModel->GetFocusImagePlot()->getYAxis()->setDrawMode1(JKQTPCADMLineTicks);

    imageDataModel->GetFocusImagePlot_ProjectionX()->getPlotter()->setUseAntiAliasingForGraphs(true);
    imageDataModel->GetFocusImagePlot_ProjectionX()->getPlotter()->setUseAntiAliasingForSystem(true);
    imageDataModel->GetFocusImagePlot_ProjectionX()->getPlotter()->setUseAntiAliasingForText(true);
    imageDataModel->GetFocusImagePlot_ProjectionX()->setGrid(false);
    imageDataModel->GetFocusImagePlot_ProjectionX()->setMousePositionShown(false);
    imageDataModel->GetFocusImagePlot_ProjectionX()->synchronizeXToMaster(imageDataModel->GetFocusImagePlot());
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

    // FIXME: use column and row stretch instead of setting minimum width and height
    layout->setColumnMinimumWidth(1, 400);
    layout->setRowMinimumHeight(0, 400);
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
    connect(view->ui->spinbox_GAIN, &QDoubleSpinBox::editingFinished, this, &Controller::OnGainChangedFromSpinbox);
    connect(this, &Controller::GainChanged, view, &FocusDiagnosticsMainWindow::OnGainChanged);
    connect(this, &Controller::GainChanged, imageDataModel, &ImageDataModel::OnGainChanged);

    // EXPOSURE TIME
    connect(view->ui->slider_EXPOSURE_TIME, &QSlider::valueChanged, this, &Controller::OnExposureTimeChangedFromSlider);
    connect(view->ui->spinbox_EXPOSURE_TIME, &QDoubleSpinBox::editingFinished, this, &Controller::OnExposureTimeChangedFromSpinbox);
    connect(this, &Controller::ExposureTimeChanged, view, &FocusDiagnosticsMainWindow::OnExposureTimeChanged);
    connect(this, &Controller::ExposureTimeChanged, imageDataModel, &ImageDataModel::OnExposureTimeChanged);

    // BEAM ENERGY
    connect(view->ui->spinbox_BEAM_ENERGY, &QDoubleSpinBox::editingFinished, this, &Controller::OnBeamEnergyChanged);
    connect(this, &Controller::BeamEnergyChanged, view, &FocusDiagnosticsMainWindow::OnBeamEnergyChanged);
    connect(this, &Controller::BeamEnergyChanged, imageDataModel, &ImageDataModel::OnBeamEnergyChanged);

    // DISPLAY MODE
    connect(view->ui->combobox_MODE, &QComboBox::currentIndexChanged, this, &Controller::OnModeChanged);
    connect(this, &Controller::ModeChanged, view, &FocusDiagnosticsMainWindow::OnModeChanged);
    connect(this, &Controller::ModeChanged, imageDataModel, &ImageDataModel::OnModeChanged);

    // FOCUS IMAGE FILE SELECT
    connect(view->ui->button_FOCUS_IMAGE_FILE_SELECT, &QPushButton::clicked, this, &Controller::OnFocusImageFileSelectButtonClicked);
    connect(this, &Controller::FocusImageFileSelected, imageDataModel, &ImageDataModel::OnFocusImageFileSelected);
    connect(this, &Controller::FocusImageFileSelected, view, &FocusDiagnosticsMainWindow::OnFocusImageFileSelected);
}

void Controller::OnGainChangedFromSlider(int gain) {
    gainInDecibels = gain;
    emit GainChanged(gainInDecibels);
}

void Controller::OnGainChangedFromSpinbox() {
    gainInDecibels = view->ui->spinbox_GAIN->value();
    emit GainChanged(gainInDecibels);
}

void Controller::OnExposureTimeChangedFromSlider(int exposureTime) {
    exposureTimeInMicroseconds = exposureTime;
    emit ExposureTimeChanged(exposureTimeInMicroseconds);
}

void Controller::OnExposureTimeChangedFromSpinbox() {
    exposureTimeInMicroseconds = view->ui->spinbox_EXPOSURE_TIME->value();
    emit ExposureTimeChanged(exposureTimeInMicroseconds);
}

void Controller::OnBeamEnergyChanged() {
    beamEnergyInMilliJoules = view->ui->spinbox_BEAM_ENERGY->value();
    emit BeamEnergyChanged(beamEnergyInMilliJoules);
}

void Controller::OnModeChanged(int index) {
    displayMode = index;
    emit ModeChanged(displayMode);
}


void Controller::OnAcquireButtonClicked() {

}

void Controller::OnFocusImageFileSelectButtonClicked() {
    auto filename = QFileDialog::getOpenFileName(view, "Select Focus Image", QDir::homePath(), "Image Files (*.tiff *.tif)");
    emit FocusImageFileSelected(filename);
}