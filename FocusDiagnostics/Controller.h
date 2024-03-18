#pragma once

// STD
#include <iostream>

// JKQtPlotter
#include <jkqtplotter/jkqtplotter.h>

// Qt
#include <QObject>
#include <QFileDialog>

// Custom
#include "focusdiagnosticsmainwindow.h"
#include "FocusDiagnostics_autogen/include/ui_focusdiagnosticsmainwindow.h"
#include "ImageDataModel.h"

class Controller : public QObject {
    Q_OBJECT
public:
    Controller();
    ~Controller() override;

    void ConnectSignalsAndSlots();
    void ShowView();
    void Initialize();

public slots:
    void OnGainChangedFromSlider(int gain);
    void OnGainChangedFromSpinbox();
    void OnExposureTimeChangedFromSlider(int exposureTime);
    void OnExposureTimeChangedFromSpinbox();
    void OnFocusImageFileSelectButtonClicked();
    void OnAcquireButtonClicked();
    void OnModeChanged(int index);
    void OnBeamEnergyChanged();
    void OnPulseDurationChanged();
    void OnBeamFWHMCalculated(double FWHMX, double FWHMY);
    void OnNormalizedVectorPotentialCalculated(double A0);

signals:
    void GainChanged(int gain) const;
    void ExposureTimeChanged(int exposureTime) const;
    void AcquireButtonClicked() const;
    void ModeChanged(int index) const;
    void BeamEnergyChanged(int beamEnergy) const;
    void PulseDurationChanged(int pulseDuration) const;
    void FocusImageFileSelected(const QString& filePath) const;
    void BeamFWHMCalculated(double FWHMX, double FWHMY) const;
    void NormalizedVectorPotentialCalculated(double A0) const;

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
};
