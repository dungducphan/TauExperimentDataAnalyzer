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
    void OnModeChanged(int index) const;
    void OnBeamEnergyChanged(int beamEnergy);
    void OnPulseDurationChanged(int pulseDuration);
    void OnFocusImageFileSelected(const QString& filePath) const;
    void OnBeamFWHMCalculated(double FWHMX, double FWHMY) const;
    void OnNormalizedVectorPotentialCalculated(double A0) const;
    void OnCamerasFound(const std::vector<std::pair<std::string, std::string>>& listOfAvailableCameras) const;
    void OnCommunicationRequestHandled(bool) const;
    void OnFoundCameraGainRange(int minGain, int maxGain) const;
    void OnFoundCameraExposureTimeRange(int minExposureTime, int maxExposureTime) const;

public:
    Ui::FocusDiagnosticsMainWindow *ui;
};
