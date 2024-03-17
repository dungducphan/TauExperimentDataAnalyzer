#pragma once

// Qt
#include <QObject>

// ROOT
#include <TASImage.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TMath.h>

// JKQtPlotter
#include <jkqtplotter/jkqtplotter.h>
#include <jkqtplotter/graphs/jkqtpimage.h>
#include <jkqtplotter/graphs/jkqtpstatisticsadaptors.h>

class ImageDataModel : public QObject {
    Q_OBJECT
public:
    explicit ImageDataModel(QObject* parent = nullptr);
    ~ImageDataModel() override;

    [[nodiscard]] JKQTPlotter* GetFocusImagePlot() const { return focusImagePlot; }
    [[nodiscard]] JKQTPlotter* GetFocusImagePlot_ProjectionX() const { return focusImagePlot_ProjectionX; }
    [[nodiscard]] JKQTPlotter* GetFocusImagePlot_ProjectionY() const { return focusImagePlot_ProjectionY; }

public slots:
    void OnGainChanged(int gain);
    void OnExposureTimeChanged(int exposureTime);
    void OnAcquireButtonClicked();
    void OnModeChanged(int index);
    void OnBeamEnergyChanged(int beamEnergy);
    void OnFocusImageFileSelected(const QString& filePath);

private:
    int gainInDecibels;
    int exposureTimeInMicroseconds;

    void JKQtPlotter_HandleFocusImageFile();

    TASImage* focusImage;
    JKQTPlotter* focusImagePlot;
    JKQTPlotter* focusImagePlot_ProjectionX;
    JKQTPlotter* focusImagePlot_ProjectionY;
};
