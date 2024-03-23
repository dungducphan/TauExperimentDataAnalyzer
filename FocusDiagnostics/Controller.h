#pragma once

// STD
#include <iostream>

// JKQtPlotter
#include <jkqtplotter/jkqtplotter.h>

// Qt
#include <QObject>
#include <QFileDialog>
#include <QIcon>

// Custom
#include <focusdiagnosticsmainwindow.h>
#include "FocusDiagnostics_autogen/include/ui_focusdiagnosticsmainwindow.h"
#include <ImageDataModel.h>
#include <ISCameraController.h>

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(QObject* parent = nullptr);
    ~Controller() override;

    void ShowView();

private:
    void ConnectSignalsAndSlots();
    void Initialize();
    void DisableCameraControls();
    void EnableCameraControls();

public slots:
    void OnGainChangedFromSlider(int gain);
    void OnGainChangedFromSpinBox();
    void OnGainReadFromHardware(int gain);
    void OnExposureTimeChangedFromSlider(int exposureTime);
    void OnExposureTimeChangedFromSpinBox();
    void OnExposureTimeReadFromHardware(int exposureTime);
    void OnFocusImageFileSelectButtonClicked();
    void OnFocusImageFileLineEditFinished();
    void OnSingleAcquisitionButtonClicked() const;
    void OnAutoAcquisitionButtonClicked();
    void OnModeChanged(int index);
    void OnBeamEnergyChanged();
    void OnPulseDurationChanged();
    void OnBeamFWHMCalculated(double FWHMX, double FWHMY);
    void OnNormalizedVectorPotentialCalculated(double A0);
    void OnCamerasFound(const std::vector<std::pair<std::string, std::string>>& namesOfAvailableCameras);
    void OnCameraSelected(const int &cameraIndex);
    void OnCameraConnectionButtonClicked() const;
    void OnCommunicationRequestHandled(bool);

signals:
    void GainChanged(int gain) const;
    void ExposureTimeChanged(int exposureTime) const;
    void ModeChanged(int index) const;
    void BeamEnergyChanged(int beamEnergy) const;
    void PulseDurationChanged(int pulseDuration) const;
    void FocusImageFileSelected(const QString& filePath) const;
    void BeamFWHMCalculated(double FWHMX, double FWHMY) const;
    void NormalizedVectorPotentialCalculated(double A0) const;
    void CamerasFound(const std::vector<std::pair<std::string, std::string>>& namesOfAvailableCameras) const;
    void CameraSelected(const int &cameraIndex) const;
    void CameraConnectionRequest() const;
    void CameraDisconnectionRequest() const;
    void CommunicationRequestHandled(bool) const;
    void FocusImageCaptureRequest() const;
    void FocusImageAutoCaptureRequest(bool) const;

private:
    FocusDiagnosticsMainWindow* view;
    ImageDataModel* imageDataModel;

    int displayMode;

    bool isAcquiring;

    int gainInDecibels;
    int exposureTimeInMicroseconds;
    int beamEnergyInMilliJoules;
    int pulseDurationInFemtoSeconds;

    double beamFWHMX;
    double beamFWHMY;
    double normalizedVectorPotential;

    ISCameraController* cameraController;
    bool isCameraConnected;
    std::vector<std::pair<std::string, std::string>> listOfAvailableCameras;
    QString selectedCameraName;
    QString selectedCameraSerial;
    bool autoCaptureEnabled;

    // Beautifiers
    QIcon* startIcon;
    QIcon* stopIcon;
    QIcon* singleIcon;
};
