#pragma once

// Qt
#include <QObject>

// TIFF
#include <tiffio.h>

// ROOT
#include <TASImage.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TMath.h>

// JKQtPlotter
#include <jkqtplotter/jkqtplotter.h>
#include <jkqtplotter/graphs/jkqtpimage.h>
#include "jkqtplotter/graphs/jkqtpfilledcurve.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"

class ImageDataModel : public QObject {
    Q_OBJECT
public:
    explicit ImageDataModel(QObject* parent = nullptr);
    ~ImageDataModel() override;

    [[nodiscard]] JKQTPlotter* GetFocusImagePlot() const { return focusImagePlot; }
    [[nodiscard]] JKQTPlotter* GetFocusImagePlot_ProjectionX() const { return focusImagePlot_ProjectionX; }
    [[nodiscard]] JKQTPlotter* GetFocusImagePlot_ProjectionY() const { return focusImagePlot_ProjectionY; }

    void SetBeamEnergy(double energy) { beamEnergyInMilliJoules = energy; }
    void SetPulseDuration(double duration) { pulseDurationInFemtoSeconds = duration; }

public slots:
    void OnGainChanged(int gain);
    void OnExposureTimeChanged(int exposureTime);
    void OnAcquireButtonClicked();
    void OnModeChanged(int index);
    void OnBeamEnergyChanged(int beamEnergy);
    void OnPulseDurationChanged(int pulseDuration);
    void OnFocusImageFileSelected(const QString& filePath);

signals:
    void BeamFWHMCalculated(double FWHMX, double FWHMY) const;
    void NormalizedVectorPotentialCalculated(double A0) const;

private:
    int gainInDecibels;
    int exposureTimeInMicroseconds;

    void JKQtPlotter_HandleFocusImageFile(const QString &filePath);
    void JKQtPlotter_HandleFocusImageFile_GetPixelDataFromImage();
    void JKQtPlotter_HandleFocusImageFile_GetCentroidPosition();
    void JKQtPlotter_HandleFocusImageFile_PlotImage();
    void JKQtPlotter_HandleFocusImageFile_PlotProjections();
    void JKQtPlotter_HandleFocusImageFile_NormalizedVectorPotential();

    double beamEnergyInMilliJoules;
    double beamSpotEnergyFraction;
    double pulseDurationInFemtoSeconds;
    double normalizedVectorPotential;

    TIFF* focusImage;
    uint32_t* pixelDataFromImage;
    uint32_t NPixelX;
    uint32_t NPixelY;
    int thresholdPixelValue;
    const double cameraSensorWidthInMillimeters;
    const double cameraSensorHeightInMillimeters;
    JKQTPlotter* focusImagePlot;
    int centroidNX; // centroid position in pixels
    int centroidNY; // centroid position in pixels
    double centroidX; // centroid position in millimeters
    double centroidY; // centroid position in millimeters
    double centroidXFWHMInMicrometers;
    double centroidYFWHMInMicrometers;
    JKQTPlotter* focusImagePlot_ProjectionX;
    JKQTPlotter* focusImagePlot_ProjectionY;
};
