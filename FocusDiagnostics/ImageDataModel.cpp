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
    auto NX = focusImage->GetWidth();
    auto NY = focusImage->GetHeight();

    // TIFF ARGB to Grayscale
    // TH2D* focusImageHistogram = new TH2D("focusImageHistogram", "focusImageHistogram", NX, 0, NX, NY, 0, NY);
    auto data = new double[NX * NY];
    for (unsigned int i = 0; i < NX; i++) {
        for (unsigned int j = 0; j < NY; j++) {
            data[i + (NY - j - 1) * NX] = TMath::Log10(focusImage->GetArgbArray()[i + j * NX] % 256);
            // focusImageHistogram->SetBinContent(i + 1, NY - j, data[i + (NY - j - 1) * NX]);
        }
    }

//    std::cout << "Start fitting!" << std::endl;
//    auto c = new TCanvas("c", "c", 600, 600);
//    auto fitFunc = new TF2("bigaus","bigaus",0,NX,0,NY);
//    fitFunc->SetParameters(200, 600, 20, 500, 20, 0.1); // "C", "x0", "dx", "y0", "dy", "rho"
//    focusImageHistogram->Draw("colz");
//    focusImageHistogram->Fit("bigaus");
//    c->SaveAs("focusImageHistogram.png");


    const auto FocusImage_DataStore = focusImagePlot->getDatastore();
    size_t focusImage_Data = FocusImage_DataStore->addCopiedImageAsColumn(data, NX, NY, "DS_FocusImage");

    // Main graph for the 2D-image
    auto graph = new JKQTPColumnMathImage(focusImagePlot);
    graph->setTitle("");
    graph->setImageColumn(focusImage_Data);
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
    graph->setShowColorBar(false);

    // add the graph to the main plot
    focusImagePlot->addGraph(graph);
    focusImagePlot->getPlotter()->setMaintainAspectRatio(true);
    focusImagePlot->getPlotter()->setMaintainAxisAspectRatio(true);
    focusImagePlot->zoomToFit();
    focusImagePlot->show();

    // resizing the projection plots
    double xmin, xmax, xmin_positive, ymin, ymax, ymin_positive;
    graph->getXMinMax(xmin, xmax, xmin_positive);
    graph->getYMinMax(ymin, ymax, ymin_positive);
    focusImagePlot_ProjectionX->setAbsoluteX(xmin,xmax);
    focusImagePlot_ProjectionY->setAbsoluteY(ymin,ymax);

//    // Compute projection X/Y data
//    int row = 0;
//    int col = 0;
//    auto px_x_data = QVector<double>(); px_x_data.reserve(NX);
//    auto px_y_data = QVector<double>(); px_y_data.reserve(NX);
//    auto py_x_data = QVector<double>(); py_x_data.reserve(NY);
//    auto py_y_data = QVector<double>(); py_y_data.reserve(NY);
//    double stepsize_x = (xmax - xmin) / NX;
//    double stepsize_y = (ymax - ymin) / NY;
//    for (int i = 0; i < focusImage->GetWidth() * focusImage->GetHeight(); ++i) {
//        row = i / (int) NX;
//        col = i % (int) NX;
//        px_y_data[col] += data[i] - minPixelValue;
//        px_x_data[col] = xmin + ((double) col) * stepsize_x;
//        std::cout << px_x_data[col] << " : " << px_y_data[col] << std::endl;
//        py_y_data[row] += data[i] - minPixelValue;
//        py_x_data[row] = ymin + ((double) row) * stepsize_y;
//    }
//    size_t focusImagePX_Y_Data = FocusImage_DataStore->addCopiedColumn(px_y_data, "DS_FocusImage_PX_Y");
//    size_t focusImagePX_X_Data = FocusImage_DataStore->addCopiedColumn(px_x_data, "DS_FocusImage_PX_X");
//    size_t focusImagePY_Y_Data = FocusImage_DataStore->addCopiedColumn(py_y_data, "DS_FocusImage_PY_Y");
//    size_t focusImagePY_X_Data = FocusImage_DataStore->addCopiedColumn(py_x_data, "DS_FocusImage_PY_X");

//    auto graph_px = new JKQTPXYLineGraph(focusImagePlot_ProjectionX);
//    graph_px->setXColumn(focusImagePX_X_Data);
//    graph_px->setYColumn(focusImagePX_Y_Data);
//    focusImagePlot_ProjectionX->addGraph(graph_px);
//    focusImagePlot_ProjectionX->zoomToFit();
//    focusImagePlot_ProjectionX->show();

//    auto graph_py = new JKQTPXYLineGraph(focusImagePlot_ProjectionY);
//    graph_py->setXColumn(focusImagePY_X_Data);
//    graph_py->setYColumn(focusImagePY_Y_Data);
//    focusImagePlot_ProjectionY->addGraph(graph_py);
//    focusImagePlot_ProjectionY->zoomToFit();
//    focusImagePlot_ProjectionY->show();
}

