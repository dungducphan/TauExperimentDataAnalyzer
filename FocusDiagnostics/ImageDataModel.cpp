#include "ImageDataModel.h"

ImageDataModel::ImageDataModel(QObject* parent) :
QObject(parent),
gainInDecibels(0),
exposureTimeInMicroseconds(0),
focusImage(nullptr),
focusImagePlot(new JKQTPlotter),
focusImagePlot_ProjectionX(new JKQTPlotter),
focusImagePlot_ProjectionY(new JKQTPlotter) {
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
    focusImage = new TASImage(filePath.toStdString().c_str(), TImage::kTiff);
    JKQtPlotter_HandleFocusImageFile();
}

void ImageDataModel::JKQtPlotter_HandleFocusImageFile() {
    // Getting the datastore from the TASImage pixel data
    JKQTPDatastore* ds = focusImagePlot->getDatastore();
    auto NX = focusImage->GetWidth();
    auto NY = focusImage->GetHeight();
    size_t cAiryDisk = ds->addCopiedImageAsColumn(focusImage->GetArgbArray(), NX, NY, "FocusImageDatastore");


    auto graph = new JKQTPColumnMathImage(focusImagePlot);
    graph->setTitle("");
    graph->setImageColumn(cAiryDisk);
    graph->setNx((int) NX);
    graph->setNy((int) NY);
    // The numbers below come from the size (in micrometers) of the image sensor on FLIR BFLY-PGE-31S4M/C-C
    double width = 7.070;
    double height = 5.300;
    // where does the image start in the plot, given in plot-axis-coordinates (bottom-left corner)
    graph->setX(-width/2.0);
    graph->setY(-height/2.0);
    // width and height of the image in plot-axis-coordinates
    graph->setWidth(width);
    graph->setHeight(height);
    // Styling
    graph->setColorPalette(JKQTPMathImageMATLAB);
    graph->setAutoImageRange(true);
    graph->setColorBarRightVisible(false);

    focusImagePlot->addGraph(graph);
    focusImagePlot->getPlotter()->setMaintainAspectRatio(true);
    focusImagePlot->getPlotter()->setMaintainAxisAspectRatio(true);
    focusImagePlot->zoomToFit();
    focusImagePlot->show();

    double xmin = 0;
    double xmax = 0;
    double xmin_positive = 0;
    double ymin = 0;
    double ymax = 0;
    double ymin_positive = 0;
    graph->getXMinMax(xmin, xmax, xmin_positive);
    graph->getYMinMax(ymin, ymax, ymin_positive);

    focusImagePlot_ProjectionY->useAsInternalDatastore(ds);
    focusImagePlot_ProjectionY->setAbsoluteX(xmin,xmax);
    focusImagePlot_ProjectionY->show();

    focusImagePlot_ProjectionX->useAsInternalDatastore(ds);
    focusImagePlot_ProjectionX->synchronizeXToMaster(focusImagePlot);
    focusImagePlot_ProjectionX->setAbsoluteY(ymin,ymax);
    focusImagePlot_ProjectionX->show();

//    jkqtpstatAddVHistogram1DAutoranged(focusImagePlot_ProjectionY->getPlotter(),  ds->begin(randomdatacoly), ds->end(randomdatacoly), 1.0, true);
//    jkqtpstatAddHHistogram1DAutoranged(plothistBottom->getPlotter(),  datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), 1.0, true);



}

