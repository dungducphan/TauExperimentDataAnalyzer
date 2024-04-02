#include <CameraControl.h>
#include <ui_CameraControl.h>

CameraControl::CameraControl(QWidget *parent) :
        QWidget(parent), ui(new Ui::CameraControl),
        imagePlotter(nullptr),
        ImageDataStore(nullptr),
        NPixelX(1024),
        NPixelY(1024) {
    ui->setupUi(this);

    // Setup plotter
    imagePlotter = new JKQTPlotter(ui->widget_IMAGE);
}

CameraControl::~CameraControl() {
    delete ui;
}

void CameraControl::OnMonitorThreadStarted() {
    ui->pushButton_CAMERACONNECT->setText("Disconnect");
    ui->pushButton_CAMERACONNECT->setStyleSheet("background-color: red");
}

void CameraControl::OnMonitorThreadFinished() {
    ui->pushButton_CAMERACONNECT->setText("Connect");
    ui->pushButton_CAMERACONNECT->setStyleSheet("background-color: green");
}

void CameraControl::OnImageReceived(const epics::pvData::shared_vector<const uint8_t>& image, const unsigned int& imageCounter) {
    std::cout << "Image received: " << imageCounter << std::endl;
    imagePlotter->clearGraphs();
    ImageDataStore = imagePlotter->getDatastore();

    size_t focusImage_Data = ImageDataStore->addCopiedImageAsColumn(image.data(), NPixelX, NPixelY, "DS_Image");
    auto graph = new JKQTPColumnMathImage(imagePlotter);
    graph->setTitle("");
    graph->setImageColumn(focusImage_Data);
    graph->setNx((int) NPixelX);
    graph->setNy((int) NPixelY);
    graph->setX(- (int) NPixelX / 2.0);
    graph->setY(- (int) NPixelY / 2.0);
    graph->setWidth((double) NPixelX);
    graph->setHeight((double) NPixelY);
    graph->setColorPalette(JKQTPMathImageMATLAB);
    graph->setAutoImageRange(true);
    graph->setShowColorBar(false);

    // add the graph to the main plot
    imagePlotter->addGraph(graph);
    imagePlotter->getPlotter()->setMaintainAspectRatio(true);
    imagePlotter->getPlotter()->setMaintainAxisAspectRatio(true);
    imagePlotter->zoomToFit();
    imagePlotter->show();
}
