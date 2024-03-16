#pragma once

#include <QMainWindow>

namespace Ui {
    class FocusDiagnosticsMainWindow;
}

class FocusDiagnosticsMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit FocusDiagnosticsMainWindow(QWidget *parent = nullptr);
    ~FocusDiagnosticsMainWindow() override;

public slots:
    void OnGainChanged(int gain) const;
    void OnExposureTimeChanged(int exposureTime) const;
    void OnAcquireButtonClicked();
    void OnModeChanged(int index) const;
    void OnBeamEnergyChanged(double beamEnergy);

public:
    Ui::FocusDiagnosticsMainWindow *ui;
};
