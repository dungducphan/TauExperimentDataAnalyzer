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

void FocusDiagnosticsMainWindow::OnAcquireButtonClicked() {

}

void FocusDiagnosticsMainWindow::OnModeChanged(int index) const {
    if (index == 0) {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (index == 1) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void FocusDiagnosticsMainWindow::OnBeamEnergyChanged(double beamEnergy) {

}
