#include "focusdiagnosticsmainwindow.h"
#include "FocusDiagnostics_autogen/include/ui_focusdiagnosticsmainwindow.h"

FocusDiagnosticsMainWindow::FocusDiagnosticsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FocusDiagnosticsMainWindow) {
    ui->setupUi(this);
}

FocusDiagnosticsMainWindow::~FocusDiagnosticsMainWindow() {
    delete ui;
}

void FocusDiagnosticsMainWindow::OnGainChanged(int gain) const {
    if (ui->slider_GAIN->value() != gain) {
        ui->slider_GAIN->setValue(gain);
    }
    if (ui->spinbox_GAIN->value() != gain) {
        ui->spinbox_GAIN->setValue(gain);
    }
}

void FocusDiagnosticsMainWindow::OnExposureTimeChanged(int exposureTime) const {
    if (ui->slider_EXPOSURE_TIME->value() != exposureTime) {
        ui->slider_EXPOSURE_TIME->setValue(exposureTime);
    }
    if (ui->spinbox_EXPOSURE_TIME->value() != exposureTime) {
        ui->spinbox_EXPOSURE_TIME->setValue(exposureTime);
    }
}

void FocusDiagnosticsMainWindow::OnModeChanged(int index) const {
    if (index == 0) {
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget->setTabEnabled(1, true);
    } else if (index == 1) {
        ui->stackedWidget->setCurrentIndex(1);
        ui->tabWidget->setTabEnabled(1, false);
    }
}

void FocusDiagnosticsMainWindow::OnBeamEnergyChanged(int beamEnergy) {
}

void FocusDiagnosticsMainWindow::OnPulseDurationChanged(int pulseDuration) {
}

void FocusDiagnosticsMainWindow::OnFocusImageFileSelected(const QString &filePath) const {
    ui->lineedit_FOCUS_IMAGE_FILENAME->setText(filePath);
}

void FocusDiagnosticsMainWindow::OnBeamFWHMCalculated(double FWHMX, double FWHMY) const {
    ui->label_BEAM_FWHM_X->setText(QString::number(FWHMX, 'd', 1));
    ui->label_BEAM_FWHM_Y->setText(QString::number(FWHMY, 'd', 1));
}

void FocusDiagnosticsMainWindow::OnNormalizedVectorPotentialCalculated(double A0) const {
    ui->label_A0->setText(QString::number(A0, 'd', 2));
}

void FocusDiagnosticsMainWindow::OnCamerasFound(const std::vector<std::pair<std::string, std::string>>& listOfAvailableCameras) const {
    ui->comboBox_CAMERA_LIST->clear();
    for (auto& elem : listOfAvailableCameras) {
        ui->comboBox_CAMERA_LIST->addItem(QString::fromStdString(elem.first));
    }
    ui->button_CAMERA_CONNECTION->setStyleSheet("background-color: green;");
}

void FocusDiagnosticsMainWindow::OnCommunicationRequestHandled(bool isCameraConnected) const {
    if (isCameraConnected) {
        ui->button_CAMERA_CONNECTION->setText("Disconnect");
        ui->button_CAMERA_CONNECTION->setStyleSheet("background-color: red;");
        ui->comboBox_CAMERA_LIST->setEnabled(false);
    } else {
        ui->button_CAMERA_CONNECTION->setText("Connect");
        ui->button_CAMERA_CONNECTION->setStyleSheet("background-color: green;");
        ui->comboBox_CAMERA_LIST->setEnabled(true);
    }
}

