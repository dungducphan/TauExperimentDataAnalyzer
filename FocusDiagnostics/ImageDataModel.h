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
    void OnBeamEnergyChanged(int beamEnergy);
    void OnPulseDurationChanged(int pulseDuration);
    void OnFocusImageFileSelected(const QString& filePath);
    void OnImageCaptured(uint32_t* pixelData, int Nx, int Ny);
    void OnImageProcessingCompleted();

signals:
    void BeamFWHMCalculated(double FWHMX, double FWHMY) const;
    void NormalizedVectorPotentialCalculated(double A0) const;
    void ImageProcessingCompleted() const;

private:

    void ClearDataFromPreviousImageFile();
    void GetPixelArrayFromImageFile();
    void ProcessImageData();
    [[nodiscard]] double FindBackgroundPixelValue() const;
    void SubtractBackground(double&);
    void CalculateThresholdPixelValue(double&);
    void CalculateBeamSpotEnergyFraction();

    void JKQtPlotter_HandleFocusImageFile_GetPixelDataFromImage();
    void GetBeamCenterPosition();
    void DrawImage();
    void DrawProjections();
    void CalculateNormalizedVectorPotential();

    double beamEnergyInMilliJoules;
    double beamSpotEnergyFraction;
    double pulseDurationInFemtoSeconds;
    double normalizedVectorPotential;

    QString imageFilePath;
    double maxPixelValue;
    TH1D* pixelValueHistogram;
    TIFF* focusImage;
    uint32_t* pixelArrayData;
    uint32_t NPixelX;
    uint32_t NPixelY;
    int thresholdPixelValue;
    double totalPixelValueOfImage;

    const double cameraSensorWidthInMicrometers;
    const double cameraSensorHeightInMicrometers;
    JKQTPlotter* focusImagePlot;
    int centroidNX; // centroid position in pixels
    int centroidNY; // centroid position in pixels
    double centroidX; // centroid position in micrometers
    double centroidY; // centroid position in micrometers
    double centroidXFWHMInMicrometers;
    double centroidYFWHMInMicrometers;
    JKQTPlotter* focusImagePlot_ProjectionX;
    JKQTPlotter* focusImagePlot_ProjectionY;
};
