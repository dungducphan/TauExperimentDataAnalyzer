#include <RCMainWindow.h>
#include <ui_RCMainWindow.h>

RCMainWindow::RCMainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::RCMainWindow),
        especPointingPlotter(new JKQTPlotter(this)),
        especSpectrumLEPlotter(new JKQTPlotter(this)),
        especSpectrumHEPlotter(new JKQTPlotter(this)),
        wfsIntensityPlotter(nullptr),
        wfsDensityPlotter(nullptr) {
    ui->setupUi(this);
    UIConfig();
}

RCMainWindow::~RCMainWindow() {
    delete ui;
}

void RCMainWindow::UIConfig() {
    // PLOTTERS CONFIG
    especPointingPlotter->getPlotter()->setUseAntiAliasingForGraphs(false);
    especPointingPlotter->getPlotter()->setUseAntiAliasingForSystem(false);
    especPointingPlotter->getPlotter()->setUseAntiAliasingForText(false);
    especPointingPlotter->setGrid(false);
    especPointingPlotter->setMousePositionShown(false);

    especSpectrumLEPlotter->getPlotter()->setUseAntiAliasingForGraphs(false);
    especSpectrumLEPlotter->getPlotter()->setUseAntiAliasingForSystem(false);
    especSpectrumLEPlotter->getPlotter()->setUseAntiAliasingForText(false);
    especSpectrumLEPlotter->setGrid(false);
    especSpectrumLEPlotter->setMousePositionShown(false);

    especSpectrumHEPlotter->getPlotter()->setUseAntiAliasingForGraphs(false);
    especSpectrumHEPlotter->getPlotter()->setUseAntiAliasingForSystem(false);
    especSpectrumHEPlotter->getPlotter()->setUseAntiAliasingForText(false);
    especSpectrumHEPlotter->setGrid(false);
    especSpectrumHEPlotter->setMousePositionShown(false);

//    wfsIntensityPlotter->getPlotter()->setUseAntiAliasingForGraphs(false);
//    wfsIntensityPlotter->getPlotter()->setUseAntiAliasingForSystem(false);
//    wfsIntensityPlotter->getPlotter()->setUseAntiAliasingForText(false);
//    wfsIntensityPlotter->setGrid(false);
//    wfsIntensityPlotter->setMousePositionShown(false);
//
//    wfsDensityPlotter->getPlotter()->setUseAntiAliasingForGraphs(false);
//    wfsDensityPlotter->getPlotter()->setUseAntiAliasingForSystem(false);
//    wfsDensityPlotter->getPlotter()->setUseAntiAliasingForText(false);
//    wfsDensityPlotter->setGrid(false);
//    wfsDensityPlotter->setMousePositionShown(false);

    // ATTACH TO ESPEC TAB
    especPointingPlotter->setParent(ui->tab_ESPEC);
    especSpectrumLEPlotter->setParent(ui->tab_ESPEC);
    especSpectrumHEPlotter->setParent(ui->tab_ESPEC);
//    wfsIntensityPlotter->setParent(ui->tab_ESPEC);
//    wfsDensityPlotter->setParent(ui->tab_ESPEC);

    // ESPEC TAB CONFIG
    ui->tab_ESPEC->setLayout(new QGridLayout());
    auto aLayout = (QGridLayout*) ui->tab_ESPEC->layout();
    aLayout->addWidget(especPointingPlotter, 0, 0, 2, 1);
    aLayout->addWidget(especSpectrumLEPlotter, 0, 1);
    aLayout->addWidget(especSpectrumHEPlotter, 1, 1);
    aLayout->setColumnStretch(0, 1);
    aLayout->setColumnStretch(1, 2);
}

void
RCMainWindow::OnImageReceived(const epics::pvData::shared_vector<const uint8_t> &image,
                              const unsigned int &monID,
                              const unsigned int &imageCounter) {
    switch (monID) {
        case 0:
            UpdatePlotter(especPointingPlotter, image, monID, imageCounter);
            break;
        case 1:
            UpdatePlotter(especSpectrumLEPlotter, image, monID, imageCounter);
            break;
        case 2:
            UpdatePlotter(especSpectrumHEPlotter, image, monID, imageCounter);
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
}

void RCMainWindow::UpdatePlotter(JKQTPlotter *plotter,
                                 const epics::pvData::shared_vector<const uint8_t> &image,
                                 const unsigned int &monID,
                                 const unsigned int &imageCounter) {

    size_t NPixelX, NPixelY;
    NPixelX = 2048;
    NPixelY = 1536;

    std::cout << "Image received: " << imageCounter << std::endl;
    
    plotter->clearGraphs();
    auto ImageDataStore = plotter->getDatastore();

    size_t focusImage_Data = ImageDataStore->addCopiedImageAsColumn(image.data(), NPixelX, NPixelY, "DS_Image");
    auto graph = new JKQTPColumnMathImage(plotter);
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
    plotter->addGraph(graph);
    plotter->getPlotter()->setMaintainAspectRatio(true);
    plotter->getPlotter()->setMaintainAxisAspectRatio(true);
    plotter->zoomToFit();
    plotter->show();
}
