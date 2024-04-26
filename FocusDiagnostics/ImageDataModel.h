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
#include <TFile.h>

// JKQtPlotter
#include <jkqtplotter/jkqtplotter.h>
#include <jkqtplotter/graphs/jkqtpimage.h>
#include <jkqtplotter/graphs/jkqtpfilledcurve.h>
#include <jkqtplotter/graphs/jkqtpgeometric.h>

#define N_DATAPOINTS 50

class ImageDataModel : public QObject {
    Q_OBJECT
public:
    explicit ImageDataModel(QObject* parent = nullptr);
    ~ImageDataModel() override;

    [[nodiscard]] JKQTPlotter* GetFocusImagePlot() const { return focusImagePlot; }
    [[nodiscard]] JKQTPlotter* GetFocusImagePlot_ProjectionX() const { return focusImagePlot_ProjectionX; }
    [[nodiscard]] JKQTPlotter* GetFocusImagePlot_ProjectionY() const { return focusImagePlot_ProjectionY; }

    [[nodiscard]] JKQTPlotter* GetNormalizedVectorPotentialTimeSeriesPlot() const { return normalizedVectorPotentialTimeSeriesPlot; }
    [[nodiscard]] JKQTPlotter* GetBeamSpotWaistFWHMTimeSeriesPlot() const { return beamSpotWaistFWHMTimeSeriesPlot; }

    void SetBeamEnergy(double energy) { beamEnergyInMilliJoules = energy; }
    void SetPulseDuration(double duration) { pulseDurationInFemtoSeconds = duration; }

public slots:
    void OnBeamEnergyChanged(int beamEnergy);
    void OnPulseDurationChanged(int pulseDuration);
    void OnFocusImageFileSelected(const QString& filePath);
    void OnImageCaptured(uint32_t* pixelData, int Nx, int Ny);
    void OnImageProcessingCompleted();
    void OnModeChanged(int index);

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

    void GetBeamCenterPosition();
    void DrawImage();
    void DrawProjections();
    void CalculateNormalizedVectorPotential();

    void CreateTimeSeriesGraphs();

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

    JKQTPlotter* normalizedVectorPotentialTimeSeriesPlot;
    JKQTPlotter* beamSpotWaistFWHMTimeSeriesPlot;
    JKQTPXYLineGraph* graph_a0;
    JKQTPXYLineGraph* graph_beamSpotFWHMX;
    JKQTPXYLineGraph* graph_beamSpotFWHMY;
    size_t NDATA;
    size_t N_DataPointsProcessed;
    std::array<double, 5000> timeIndex;
    std::array<double, 5000> normalizedVectorPotentialTimeSeries;
    std::array<double, 5000> beamSpotWaistFWHMXTimeSeries;
    std::array<double, 5000> beamSpotWaistFWHMYTimeSeries;
};
