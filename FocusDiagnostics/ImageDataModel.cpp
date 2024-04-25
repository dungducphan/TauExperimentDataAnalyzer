#include "ImageDataModel.h"

ImageDataModel::ImageDataModel(QObject* parent) :
        QObject(parent),
        focusImage(nullptr),
        focusImagePlot(new JKQTPlotter),
        focusImagePlot_ProjectionX(new JKQTPlotter),
        focusImagePlot_ProjectionY(new JKQTPlotter),
        normalizedVectorPotentialTimeSeriesPlot(new JKQTPlotter),
        beamSpotWaistFWHMTimeSeriesPlot(new JKQTPlotter),
        pixelArrayData(nullptr),
        NPixelX(0),
        NPixelY(0),
        thresholdPixelValue(0),
        cameraSensorWidthInMicrometers(299.472),  // taken from Phillip Franke's calibration code
        cameraSensorHeightInMicrometers(224.604), // taken from Phillip Franke's calibration code
        centroidX(0),
        centroidY(0),
        centroidNX(0),
        centroidNY(0),
        centroidXFWHMInMicrometers(0),
        centroidYFWHMInMicrometers(0),
        beamEnergyInMilliJoules(0),
        pulseDurationInFemtoSeconds(0),
        beamSpotEnergyFraction(0),
        normalizedVectorPotential(0),
        maxPixelValue(0),
        pixelValueHistogram(new TH1D("PixelValueHistogram", "Pixel Value Histogram", 256, -0.5, 255.5)),
        totalPixelValueOfImage(0),
        NDATA(N_DATAPOINTS),
        graph_a0(nullptr),
        graph_beamSpotFWHMX(nullptr),
        graph_beamSpotFWHMY(nullptr),
        N_DataPointsProcessed(0) {
    for (size_t i = 0; i < 50; i++) {
        timeIndex[i] = (double) i;
    }
    normalizedVectorPotentialTimeSeries.fill(0);
    beamSpotWaistFWHMXTimeSeries.fill(0);
    beamSpotWaistFWHMYTimeSeries.fill(0);

    CreateTimeSeriesGraphs();
}

ImageDataModel::~ImageDataModel() = default;

void ImageDataModel::CreateTimeSeriesGraphs() {
    size_t T_1 = normalizedVectorPotentialTimeSeriesPlot->getDatastore()->addColumn(timeIndex.data(), NDATA, "T_1");
    size_t TS_A0 = normalizedVectorPotentialTimeSeriesPlot->getDatastore()->addColumn(normalizedVectorPotentialTimeSeries.data(), NDATA, "A0");
    size_t T_2 = beamSpotWaistFWHMTimeSeriesPlot->getDatastore()->addColumn(timeIndex.data(), NDATA, "T_2");
    size_t TS_SpotSizeX = beamSpotWaistFWHMTimeSeriesPlot->getDatastore()->addColumn(beamSpotWaistFWHMXTimeSeries.data(), NDATA, "FWHMX");
    size_t TS_SpotSizeY = beamSpotWaistFWHMTimeSeriesPlot->getDatastore()->addColumn(beamSpotWaistFWHMYTimeSeries.data(), NDATA, "FWHMY");

    graph_a0 = new JKQTPXYLineGraph(normalizedVectorPotentialTimeSeriesPlot);
    graph_a0->setXColumn(T_1);
    graph_a0->setYColumn(TS_A0);
    graph_a0->setTitle(QObject::tr("Normalized Vector Potential"));
    graph_a0->setLineWidth(1);
    graph_a0->setSymbolType(JKQTPNoSymbol);
    normalizedVectorPotentialTimeSeriesPlot->addGraph(graph_a0);

    graph_beamSpotFWHMX = new JKQTPXYLineGraph(beamSpotWaistFWHMTimeSeriesPlot);
    graph_beamSpotFWHMX->setXColumn(T_2);
    graph_beamSpotFWHMX->setYColumn(TS_SpotSizeX);
    graph_beamSpotFWHMX->setTitle(QObject::tr("Beam Spot FHWM X"));
    graph_beamSpotFWHMX->setLineWidth(1);
    graph_beamSpotFWHMX->setSymbolType(JKQTPNoSymbol);
    beamSpotWaistFWHMTimeSeriesPlot->addGraph(graph_beamSpotFWHMX);

    graph_beamSpotFWHMY = new JKQTPXYLineGraph(beamSpotWaistFWHMTimeSeriesPlot);
    graph_beamSpotFWHMY->setXColumn(T_2);
    graph_beamSpotFWHMY->setYColumn(TS_SpotSizeY);
    graph_beamSpotFWHMY->setTitle(QObject::tr("Beam Spot FHWM Y"));
    graph_beamSpotFWHMY->setLineWidth(1);
    graph_beamSpotFWHMY->setSymbolType(JKQTPNoSymbol);
    beamSpotWaistFWHMTimeSeriesPlot->addGraph(graph_beamSpotFWHMY);

    // Scale the plot so the graph is contained
    normalizedVectorPotentialTimeSeriesPlot->setX(timeIndex[0], timeIndex[NDATA - 1]);
    normalizedVectorPotentialTimeSeriesPlot->setY(0,3);
    beamSpotWaistFWHMTimeSeriesPlot->setX(timeIndex[0], timeIndex[NDATA - 1]);
    beamSpotWaistFWHMTimeSeriesPlot->setY(0,30);

    // Show
    normalizedVectorPotentialTimeSeriesPlot->show();
    beamSpotWaistFWHMTimeSeriesPlot->show();
}

void ImageDataModel::OnBeamEnergyChanged(int beamEnergy) {
    beamEnergyInMilliJoules = beamEnergy;
    CalculateNormalizedVectorPotential();
}

void ImageDataModel::OnPulseDurationChanged(int pulseDurations) {
    pulseDurationInFemtoSeconds = pulseDurations;
    CalculateNormalizedVectorPotential();
}

void ImageDataModel::ClearDataFromPreviousImageFile() {
    focusImagePlot->clearGraphs();
    focusImagePlot_ProjectionX->clearGraphs();
    focusImagePlot_ProjectionY->clearGraphs();
    beamSpotEnergyFraction = 0;
    maxPixelValue = 0;
    thresholdPixelValue = 0;
    totalPixelValueOfImage = 0;
    pixelValueHistogram->Reset();
}

void ImageDataModel::GetPixelArrayFromImageFile() {
    focusImage = TIFFOpen(imageFilePath.toStdString().c_str(), "r");
    if (focusImage) {
        // Get pixel data from the image
        TIFFGetField(focusImage, TIFFTAG_IMAGEWIDTH, &NPixelX);
        TIFFGetField(focusImage, TIFFTAG_IMAGELENGTH, &NPixelY);
        pixelArrayData = (uint32_t *) _TIFFmalloc(NPixelX * NPixelY * sizeof(uint32_t));
        if (pixelArrayData != nullptr) {
            if (TIFFReadRGBAImage(focusImage, NPixelX, NPixelY, pixelArrayData, 0)) {
                for (size_t i = 0; i < NPixelX * NPixelY; i++) {
                    pixelArrayData[i] = pixelArrayData[i] % 256;
                    pixelValueHistogram->Fill(pixelArrayData[i]);
                }
            }
        }
        TIFFClose(focusImage);
    }
}

double ImageDataModel::FindBackgroundPixelValue() const {
    bool foundMinBin = false;
    double MinBin = 255.5;
    for (int i = 0; i < pixelValueHistogram->GetSize(); i++) {
        if (foundMinBin) break;
        if (pixelValueHistogram->GetBinContent(i) == 0) continue;
        if (pixelValueHistogram->GetBinContent(i) > 0) {
            MinBin = pixelValueHistogram->GetBinCenter(i);
            foundMinBin = true;
        }
    }

    return MinBin;
}

void ImageDataModel::SubtractBackground(double& backgroundPixelValue) {
    // This function performs 4 tasks at once (which goes against the Single Responsibility Principle but
    // is done for performance reasons):
    // 1. Subtract the background pixel value from the pixel array data.
    // 2. Calculate the total pixel value of the image.
    // 3. Calculate the maximum pixel value of the image.
    // 4. Fill the pixel value histogram.

    pixelValueHistogram->Reset();
    totalPixelValueOfImage = 0;
    maxPixelValue = 0;
    for (unsigned int i = 0; i < NPixelX * NPixelY; i++) {
        pixelArrayData[i] -= (uint32_t) backgroundPixelValue;
        totalPixelValueOfImage += pixelArrayData[i];
        pixelValueHistogram->Fill(pixelArrayData[i]);
        if (pixelArrayData[i] > maxPixelValue) {
            maxPixelValue = pixelArrayData[i];
        }
    }
}

void ImageDataModel::CalculateThresholdPixelValue(double& thresholdFraction) {
    thresholdPixelValue = (int) (thresholdFraction * maxPixelValue);
}

void ImageDataModel::CalculateBeamSpotEnergyFraction() {
    double totalPixelValueOfBeamSpot = 0;
    for (int i = 0; i < pixelValueHistogram->GetSize(); i++) {
        if (pixelValueHistogram->GetBinCenter(i) >= maxPixelValue / 2.) {
            totalPixelValueOfBeamSpot += pixelValueHistogram->GetBinContent(i) * pixelValueHistogram->GetBinCenter(i);
        }
    }

    beamSpotEnergyFraction = totalPixelValueOfBeamSpot / totalPixelValueOfImage;
}

void ImageDataModel::GetBeamCenterPosition() {
    auto profile2DOfPixelsAboveThreshold = new TH2D("PixelsWithValueAboveThreshold", "PixelsWithValueAboveThreshold", NPixelX, 0, NPixelX, NPixelY, 0, NPixelY);
    for (size_t i = 0; i < NPixelX * NPixelY; i++) {
        if (pixelArrayData[i] >= thresholdPixelValue) profile2DOfPixelsAboveThreshold->Fill(i % NPixelX, i / NPixelX);
    }
    centroidNX = TMath::Nint(profile2DOfPixelsAboveThreshold->ProjectionX()->GetMean());
    centroidNY = TMath::Nint(profile2DOfPixelsAboveThreshold->ProjectionY()->GetMean());
    centroidX = -cameraSensorWidthInMicrometers / 2.0 + centroidNX * cameraSensorWidthInMicrometers / NPixelX;
    centroidY = -cameraSensorHeightInMicrometers / 2.0 + centroidNY * cameraSensorHeightInMicrometers / NPixelY;
    profile2DOfPixelsAboveThreshold->Delete();
}

void ImageDataModel::DrawImage() {
    // Main graph for the 2D-image
    const auto FocusImage_DataStore = focusImagePlot->getDatastore();
    size_t focusImage_Data = FocusImage_DataStore->addCopiedImageAsColumn(pixelArrayData, NPixelX, NPixelY, "DS_FocusImage");
    auto graph = new JKQTPColumnMathImage(focusImagePlot);
    graph->setTitle("");
    graph->setImageColumn(focusImage_Data);
    graph->setNx((int) NPixelX);
    graph->setNy((int) NPixelY);
    graph->setX(-cameraSensorWidthInMicrometers / 2.0);
    graph->setY(-cameraSensorHeightInMicrometers / 2.0);
    graph->setWidth(cameraSensorWidthInMicrometers);
    graph->setHeight(cameraSensorHeightInMicrometers);
    graph->setColorPalette(JKQTPMathImageMATLAB);
    graph->setAutoImageRange(true);
    graph->setShowColorBar(false);

    // add the graph to the main plot
    focusImagePlot->addGraph(graph);
    focusImagePlot->getPlotter()->setMaintainAspectRatio(true);
    focusImagePlot->getPlotter()->setMaintainAxisAspectRatio(true);
    focusImagePlot->zoomToFit();

    // add centroid lines
    auto lX = new JKQTPGeoLine(focusImagePlot, centroidX, -cameraSensorHeightInMicrometers / 2.0, centroidX, cameraSensorHeightInMicrometers / 2.0);
    auto lY = new JKQTPGeoLine(focusImagePlot, -cameraSensorWidthInMicrometers / 2.0, centroidY, cameraSensorWidthInMicrometers / 2.0, centroidY);
    lX->setStyle(QColor("red"), 1);
    lY->setStyle(QColor("red"), 1);
    focusImagePlot->addGraph(lX);
    focusImagePlot->addGraph(lY);
    focusImagePlot->show();
}

void ImageDataModel::DrawProjections() {
    // Compute projection X-Y data
    double xmin, xmax, xmin_positive, ymin, ymax, ymin_positive;
    focusImagePlot->getGraph(0)->getXMinMax(xmin, xmax, xmin_positive);
    focusImagePlot->getGraph(0)->getYMinMax(ymin, ymax, ymin_positive);
    int row = 0;
    int col = 0;
    auto px_x_data = new double[NPixelX];
    auto px_y_data = new double[NPixelX];
    auto py_x_data = new double[NPixelY];
    auto py_y_data = new double[NPixelY];
    double stepSize_x = (xmax - xmin) / (NPixelX - 1);
    double stepSize_y = (ymax - ymin) / (NPixelY - 1);
    for (int i = 0; i < NPixelX * NPixelY; i++) {
        if (i / NPixelX == centroidNY) {
            col = i % (int) NPixelX;
            px_y_data[col] = pixelArrayData[i];
            px_x_data[col] = xmin + ((double) col) * stepSize_x;
        }
        if (i % NPixelX == centroidNX) {
            row = i / (int) NPixelX;
            py_x_data[row] = pixelArrayData[i];
            py_y_data[row] = ymin + ((double) row) * stepSize_y;
        }
    }

    // Prep Gaussian Fit to find the FWHM of the centroid
    auto hPx = new TH1D("hPx", "Projection X", NPixelX, xmin, xmax);
    auto hPy = new TH1D("hPy", "Projection Y", NPixelY, ymin, ymax);
    for (int i = 0; i < NPixelX; i++) {
        hPx->SetBinContent(hPx->FindBin(px_x_data[i]), px_y_data[i]);
    }
    for (int i = 0; i < NPixelY; i++) {
        hPy->SetBinContent(hPy->FindBin(py_y_data[i]), py_x_data[i]);
    }

    // Fit the projection X
    auto f1 = new TF1("f1", "gaus", xmin, xmax);
    f1->SetParameters(10, centroidX, 10);
    //    auto cx = new TCanvas("cx", "cx", 800, 800);
    //    cx->cd();
    //    hPx->Draw();
    hPx->Fit("f1", "0RQ"); // 0 = do not plot, R = fit range, Q = quiet, do not print results
    //    cx->SaveAs("hPx.png");
    centroidXFWHMInMicrometers = f1->GetParameter(2) * 2.355;

    // Fit the projection Y
    auto f2 = new TF1("f2", "gaus", ymin, ymax);
    f2->SetParameters(10, centroidY, 10);
    //    auto cy = new TCanvas("cy", "cy", 800, 800);
    //    cy->cd();
    //    hPy->Draw();
    hPy->Fit("f2", "0RQ"); // 0 = do not plot, R = fit range, Q = quiet, do not print results
    //    cy->SaveAs("hPy.png");
    centroidYFWHMInMicrometers = f2->GetParameter(2) * 2.355;

    // Emit the FWHM of the centroid
    hPx->Delete();
    hPy->Delete();
    emit BeamFWHMCalculated(centroidXFWHMInMicrometers, centroidYFWHMInMicrometers);

    // Draw the ellipse that indicates the beam spot (FWHM) area
    auto beamSpotEllipse = new JKQTPGeoEllipse(focusImagePlot, centroidX, centroidY, centroidXFWHMInMicrometers, centroidYFWHMInMicrometers);
    focusImagePlot->addGraph(beamSpotEllipse);
    QColor whiteFill("white");
    whiteFill.setAlphaF(0.1);
    beamSpotEllipse->setStyle(QColor("red"), 1, Qt::SolidLine, whiteFill, Qt::SolidPattern);

    // Plot the projections
    const auto FocusImagePX_DataStore = focusImagePlot_ProjectionX->getDatastore();
    size_t focusImagePX_Y_Data = FocusImagePX_DataStore->addColumn(px_y_data, NPixelX, "DS_FocusImage_PX_Y");
    size_t focusImagePX_X_Data = FocusImagePX_DataStore->addColumn(px_x_data, NPixelX, "DS_FocusImage_PX_X");
    const auto FocusImagePY_DataStore = focusImagePlot_ProjectionY->getDatastore();
    size_t focusImagePY_Y_Data = FocusImagePY_DataStore->addColumn(py_y_data, NPixelY, "DS_FocusImage_PY_Y");
    size_t focusImagePY_X_Data = FocusImagePY_DataStore->addColumn(py_x_data, NPixelY, "DS_FocusImage_PY_X");

    auto graph_px = new JKQTPFilledCurveXGraph(focusImagePlot_ProjectionX);
    graph_px->setXColumn(focusImagePX_X_Data);
    graph_px->setYColumn(focusImagePX_Y_Data);
    graph_px->setColor(QColor("red"));
    focusImagePlot_ProjectionX->addGraph(graph_px);
    // Plot the arrow indication of beam spot FWHM
    auto arrowX = new JKQTPGeoArrow(focusImagePlot_ProjectionX, centroidX-centroidXFWHMInMicrometers/2.,  maxPixelValue/2., centroidX+centroidXFWHMInMicrometers/2., maxPixelValue/2., JKQTPFilledArrow, JKQTPFilledArrow);
    arrowX->setStyle(QColor("red"), 1);
    focusImagePlot_ProjectionX->addGraph(arrowX);
    focusImagePlot_ProjectionX->zoomToFit();
    focusImagePlot_ProjectionX->show();

    auto graph_py = new JKQTPFilledCurveYGraph(focusImagePlot_ProjectionY);
    graph_py->setXColumn(focusImagePY_X_Data);
    graph_py->setYColumn(focusImagePY_Y_Data);
    graph_py->setColor(QColor("red"));
    focusImagePlot_ProjectionY->addGraph(graph_py);
    // Plot the arrow indication of beam spot FWHM
    auto arrowY = new JKQTPGeoArrow(focusImagePlot_ProjectionY, maxPixelValue/2., centroidY-centroidYFWHMInMicrometers/2.,  maxPixelValue/2., centroidY+centroidYFWHMInMicrometers/2., JKQTPFilledArrow, JKQTPFilledArrow);
    arrowY->setStyle(QColor("red"), 1);
    focusImagePlot_ProjectionY->addGraph(arrowY);
    focusImagePlot_ProjectionY->zoomToFit();
    focusImagePlot_ProjectionY->show();

    focusImagePlot->zoom(ymin, ymax, ymin, ymax);
}

void ImageDataModel::CalculateNormalizedVectorPotential() {
    // Look at Equation 7 in https://cds.cern.ch/record/2203636/files/1418884_207-230.pdf
    const double beamSpotEnergyInJoules = beamEnergyInMilliJoules * beamSpotEnergyFraction * 1e-3;
    const double beamCentralAreaInMicrometerSquared = TMath::Pi() * centroidXFWHMInMicrometers * centroidYFWHMInMicrometers / 4.0;
    const double intensityInWattsPerCentimeterSquared = beamSpotEnergyInJoules / (beamCentralAreaInMicrometerSquared * pulseDurationInFemtoSeconds * 1e-15 * 1e-8);
    const double wavelengthInMicrometers = 0.8;
    normalizedVectorPotential = 0.855E-9 * TMath::Sqrt(intensityInWattsPerCentimeterSquared) * wavelengthInMicrometers;

    emit NormalizedVectorPotentialCalculated(normalizedVectorPotential);
}

void ImageDataModel::ProcessImageData() {
    double MinPxVal = FindBackgroundPixelValue();
    SubtractBackground(MinPxVal);
    double thresholdFraction = 0.95;
    CalculateThresholdPixelValue(thresholdFraction);
    CalculateBeamSpotEnergyFraction();
    GetBeamCenterPosition();
    DrawImage();
    DrawProjections();
    CalculateNormalizedVectorPotential();
}

void ImageDataModel::OnFocusImageFileSelected(const QString &filePath) {
    if (filePath.endsWith(".tiff")) {
        imageFilePath = filePath;
        ClearDataFromPreviousImageFile();
        GetPixelArrayFromImageFile();
        ProcessImageData();
        if (pixelArrayData) _TIFFfree(pixelArrayData);
    } else {
        std::cerr << "Invalid file format. Please select a .tiff file." << std::endl;
    }
}

void ImageDataModel::OnImageCaptured(uint32_t *pixelData, int Nx, int Ny) {
    ClearDataFromPreviousImageFile();

    NPixelX = Nx;
    NPixelY = Ny;
    if (pixelArrayData) free(pixelArrayData);
    pixelArrayData = pixelData;
    for (size_t i = 0; i < NPixelX * NPixelY; i++) {
        pixelValueHistogram->Fill(pixelArrayData[i]);
    }

    ProcessImageData();

    pixelArrayData = nullptr;
    emit ImageProcessingCompleted();
}

void ImageDataModel::OnImageProcessingCompleted() {
    if (N_DataPointsProcessed < NDATA) {
        normalizedVectorPotentialTimeSeries[N_DataPointsProcessed] = normalizedVectorPotential;
        beamSpotWaistFWHMXTimeSeries[N_DataPointsProcessed] = centroidXFWHMInMicrometers;
        beamSpotWaistFWHMYTimeSeries[N_DataPointsProcessed] = centroidYFWHMInMicrometers;
    } else {
        // Move old data 1 slot to the left
        for (size_t i = 0; i < NDATA - 1; i++) {
            timeIndex[i] = timeIndex[i + 1];
            normalizedVectorPotentialTimeSeries[i] = normalizedVectorPotentialTimeSeries[i + 1];
            beamSpotWaistFWHMXTimeSeries[i] = beamSpotWaistFWHMXTimeSeries[i + 1];
            beamSpotWaistFWHMYTimeSeries[i] = beamSpotWaistFWHMYTimeSeries[i + 1];
        }

        // Adding new data point
        timeIndex[NDATA - 1] = (double) N_DataPointsProcessed;
        normalizedVectorPotentialTimeSeries[NDATA - 1] = normalizedVectorPotential;
        beamSpotWaistFWHMXTimeSeries[NDATA - 1] = centroidXFWHMInMicrometers;
        beamSpotWaistFWHMYTimeSeries[NDATA - 1] = centroidYFWHMInMicrometers;
    }

    // Update range and redraw
    normalizedVectorPotentialTimeSeriesPlot->setX(timeIndex[0], timeIndex[NDATA-1]);
    normalizedVectorPotentialTimeSeriesPlot->redrawPlot();
    beamSpotWaistFWHMTimeSeriesPlot->setX(timeIndex[0], timeIndex[NDATA-1]);
    beamSpotWaistFWHMTimeSeriesPlot->redrawPlot();

    // Update the number of data points processed
    N_DataPointsProcessed++;
}

void ImageDataModel::OnModeChanged(int index) {
    ClearDataFromPreviousImageFile();
    pixelArrayData = nullptr;
}

