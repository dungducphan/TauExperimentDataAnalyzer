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

signals:
    void GainChanged(int gain) const;
    void ExposureTimeChanged(int exposureTime) const;
    void AcquireButtonClicked() const;
    void ModeChanged(int index) const;
    void BeamEnergyChanged(int beamEnergy) const;
    void FocusImageFileSelected(const QString& filePath) const;

private:
    FocusDiagnosticsMainWindow* view;
    ImageDataModel* imageDataModel;

    int displayMode;

    bool isAcquiring;

    int gainInDecibels;
    int exposureTimeInMicroseconds;
    int beamEnergyInMilliJoules;
};