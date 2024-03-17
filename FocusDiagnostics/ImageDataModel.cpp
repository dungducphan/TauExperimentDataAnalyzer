#include "ImageDataModel.h"

ImageDataModel::ImageDataModel(QObject* parent) :
QObject(parent),
gainInDecibels(0),
exposureTimeInMicroseconds(0),
focusImage(nullptr),
focusImagePlot(new JKQTPlotter),
focusImagePlot_ProjectionX(new JKQTPlotter),
focusImagePlot_ProjectionY(new JKQTPlotter),
pixelDataFromImage(nullptr),
NPixelX(0),
NPixelY(0),
thresholdPixelValue(0),
cameraSensorWidthInMillimeters(7.070),
cameraSensorHeightInMillimeters(5.300),
centroidX(0),
centroidY(0),
centroidNX(0),
centroidNY(0) {
}

ImageDataModel::~ImageDataModel() = default;

void ImageDataModel::OnGainChanged(int gain) {
    gainInDecibels = gain;
}

void ImageDataModel::OnExposureTimeChanged(int exposureTime) {
    exposureTimeInMicroseconds = exposureTime;
}

void ImageDataModel::OnAcquireButtonClicked() {

}

void ImageDataModel::OnModeChanged(int index) {

}

void ImageDataModel::OnBeamEnergyChanged(int beamEnergy) {

}

void ImageDataModel::OnFocusImageFileSelected(const QString &filePath) {
    // Clear the previous image
    focusImagePlot->clearGraphs();
    focusImagePlot_ProjectionX->clearGraphs();
    focusImagePlot_ProjectionY->clearGraphs();

    // Handle new image file
    JKQtPlotter_HandleFocusImageFile(filePath);
}

void ImageDataModel::JKQtPlotter_HandleFocusImageFile_GetPixelDataFromImage() {
    // Get pixel data from the image
    auto pixelValueHistogram = new TH1D("PixelValueHistogram", "Pixel Value Histogram", 256, -0.5, 255.5);
    TIFFGetField(focusImage, TIFFTAG_IMAGEWIDTH, &NPixelX);
    TIFFGetField(focusImage, TIFFTAG_IMAGELENGTH, &NPixelY);
    pixelDataFromImage = (uint32_t *) _TIFFmalloc(NPixelX * NPixelY * sizeof(uint32_t));
    if (pixelDataFromImage != nullptr) {
        if (TIFFReadRGBAImage(focusImage, NPixelX, NPixelY, pixelDataFromImage, 0)) {
            for (size_t i = 0; i < NPixelX * NPixelY; i++) {
                pixelDataFromImage[i] = pixelDataFromImage[i] % 256;
                pixelValueHistogram->Fill(pixelDataFromImage[i]);
            }
        }
    }
    TIFFClose(focusImage);

    // Find the maximum and minimum pixel values
    bool foundMaxBin = false;
    bool foundMinBin = false;
    double MaxBin = -0.5;
    double MinBin = 255.5;
    for (unsigned int i = pixelValueHistogram->GetSize() - 1; i > 0; i--) {
        if (foundMaxBin) break;
        if (pixelValueHistogram->GetBinContent(i) == 0) continue;
        if (pixelValueHistogram->GetBinContent(i) > 0) {
            MaxBin = pixelValueHistogram->GetBinCenter(i);
            foundMaxBin = true;
        }
    }
    for (unsigned int i = 0; i < pixelValueHistogram->GetSize(); i++) {
        if (foundMinBin) break;
        if (pixelValueHistogram->GetBinContent(i) == 0) continue;
        if (pixelValueHistogram->GetBinContent(i) > 0) {
            MinBin = pixelValueHistogram->GetBinCenter(i);
            foundMinBin = true;
        }
    }

    // Background subtraction
    for (unsigned int i = 0; i < NPixelX * NPixelY; i++) {
        pixelDataFromImage[i] -= MinBin;
    }
    MaxBin -= MinBin;
    MinBin = 0;
    thresholdPixelValue = (int) (0.95 * (MaxBin - MinBin) + MinBin);
};

void ImageDataModel::JKQtPlotter_HandleFocusImageFile_GetCentroidPosition() {
    auto centroidHist = new TH2D("CentroidHistogram", "Centroid Histogram", NPixelX, 0, NPixelX, NPixelY, 0, NPixelY);
    for (size_t i = 0; i < NPixelX * NPixelY; i++) {
        if (pixelDataFromImage[i] >= thresholdPixelValue) {
            centroidHist->Fill(i % NPixelX, i / NPixelX);
        }
    }
    centroidNX = TMath::Nint(centroidHist->ProjectionX()->GetMean());
    centroidNY = TMath::Nint(centroidHist->ProjectionY()->GetMean());
    centroidX = -cameraSensorWidthInMillimeters/2.0 + centroidNX * cameraSensorWidthInMillimeters / NPixelX;
    centroidY = -cameraSensorHeightInMillimeters/2.0 + centroidNY * cameraSensorHeightInMillimeters / NPixelY;
}

void ImageDataModel::JKQtPlotter_HandleFocusImageFile_PlotImage() {
    // Main graph for the 2D-image
    const auto FocusImage_DataStore = focusImagePlot->getDatastore();
    size_t focusImage_Data = FocusImage_DataStore->addCopiedImageAsColumn(pixelDataFromImage, NPixelX, NPixelY, "DS_FocusImage");
    auto graph = new JKQTPColumnMathImage(focusImagePlot);
    graph->setTitle("");
    graph->setImageColumn(focusImage_Data);
    graph->setNx((int) NPixelX);
    graph->setNy((int) NPixelY);
    graph->setX(-cameraSensorWidthInMillimeters/2.0);
    graph->setY(-cameraSensorHeightInMillimeters/2.0);
    graph->setWidth(cameraSensorWidthInMillimeters);
    graph->setHeight(cameraSensorHeightInMillimeters);
    graph->setColorPalette(JKQTPMathImageMATLAB);
    graph->setAutoImageRange(true);
    graph->setShowColorBar(false);

    // add the graph to the main plot
    focusImagePlot->addGraph(graph);
    focusImagePlot->getPlotter()->setMaintainAspectRatio(true);
    focusImagePlot->getPlotter()->setMaintainAxisAspectRatio(true);
    focusImagePlot->zoomToFit();

    // add centroid lines
    auto lX = new JKQTPGeoLine(focusImagePlot, centroidX,  -cameraSensorHeightInMillimeters/2.0, centroidX, cameraSensorHeightInMillimeters/2.0);
    auto lY = new JKQTPGeoLine(focusImagePlot, -cameraSensorWidthInMillimeters/2.0,  centroidY, cameraSensorWidthInMillimeters/2.0, centroidY);
    lX->setStyle(QColor("red"), 1);
    lY->setStyle(QColor("red"), 1);
    focusImagePlot->addGraph(lX);
    focusImagePlot->addGraph(lY);
    focusImagePlot->show();
}

void ImageDataModel::JKQtPlotter_HandleFocusImageFile_PlotProjections() {
    // Compute projection X data
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
            px_y_data[col] = pixelDataFromImage[i];
            px_x_data[col] = xmin + ((double) col) * stepSize_x;
        }
        if (i % NPixelX == centroidNX) {
            row = i / (int) NPixelX;
            py_x_data[row] = pixelDataFromImage[i];
            py_y_data[row] = ymin + ((double) row) * stepSize_y;
        }
    }
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
    focusImagePlot_ProjectionX->zoomToFit();
    focusImagePlot_ProjectionX->show();

    auto graph_py = new JKQTPFilledCurveYGraph(focusImagePlot_ProjectionY);
    graph_py->setXColumn(focusImagePY_X_Data);
    graph_py->setYColumn(focusImagePY_Y_Data);
    graph_py->setColor(QColor("red"));
    focusImagePlot_ProjectionY->addGraph(graph_py);
    focusImagePlot_ProjectionY->zoomToFit();
    focusImagePlot_ProjectionY->show();

    focusImagePlot->zoom(ymin, ymax, ymin, ymax);
}

void ImageDataModel::JKQtPlotter_HandleFocusImageFile(const QString &filePath) {
    focusImage = TIFFOpen(filePath.toStdString().c_str(), "r");
    if (focusImage) {
        JKQtPlotter_HandleFocusImageFile_GetPixelDataFromImage();
        JKQtPlotter_HandleFocusImageFile_GetCentroidPosition();
        JKQtPlotter_HandleFocusImageFile_PlotImage();
        JKQtPlotter_HandleFocusImageFile_PlotProjections();
        _TIFFfree(pixelDataFromImage);
    }
}

