#include "Controller.h"

Controller::Controller() :
        displayMode(0),
        isAcquiring(false),
        gainInDecibels(0),
        exposureTimeInMicroseconds(0),
        beamEnergyInMilliJoules(0) {
    view = new FocusDiagnosticsMainWindow();
    view->ui->combobox_MODE->setCurrentIndex(displayMode);
    imageDataModel = new ImageDataModel();
}

Controller::~Controller() {
    delete view;
    delete imageDataModel;
}

void Controller::InitView() {

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

    // EXPOSURE TIME
    connect(view->ui->slider_EXPOSURE_TIME, &QSlider::valueChanged, this, &Controller::OnExposureTimeChangedFromSlider);
    connect(view->ui->spinbox_EXPOSURE_TIME, &QDoubleSpinBox::editingFinished, this, &Controller::OnExposureTimeChangedFromSpinbox);
    connect(this, &Controller::ExposureTimeChanged, view, &FocusDiagnosticsMainWindow::OnExposureTimeChanged);

    // BEAM ENERGY
    connect(view->ui->spinbox_BEAM_ENERGY, &QDoubleSpinBox::editingFinished, this, &Controller::OnBeamEnergyChanged);

    // DISPLAY MODE
    connect(view->ui->combobox_MODE, &QComboBox::currentIndexChanged, this, &Controller::OnModeChanged);
    connect(this, &Controller::ModeChanged, view, &FocusDiagnosticsMainWindow::OnModeChanged);
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
