#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/graphs/jkqtpstatisticsadaptors.h"
#include "jkqtplotter/graphs/jkqtpgeometric.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtmath/jkqtpstatisticstools.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <random>
#include <cmath>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QElapsedTimer timer;

    // 1. create a window with several plotters and get a pointer to the internal datastores (for convenience)
    QWidget mainWidget;
    QGridLayout* lay;
    mainWidget.setLayout(lay=new QGridLayout);
    JKQTPlotter* plothist=new JKQTPlotter(&mainWidget);
    lay->addWidget(plothist,0,1);
    plothist->getPlotter()->setPlotLabel("Histograms");
    JKQTPDatastore* datastore1=plothist->getDatastore();
    JKQTPlotter* plothistLeft=new JKQTPlotter(datastore1, &mainWidget);
    lay->addWidget(plothistLeft,0,0);
    JKQTPlotter* plothistBottom=new JKQTPlotter(datastore1, &mainWidget);
    lay->addWidget(plothistBottom,1,1);
    plothistLeft->synchronizeYToMaster(plothist);
    plothistLeft->setAbsoluteX(0,1);
    plothistLeft->getXAxis()->setInverted(true);
    plothistLeft->getXAxis()->setShowZeroAxis(false);
    plothistLeft->getYAxis()->setShowZeroAxis(false);
    plothistLeft->getYAxis()->setDrawMode1(JKQTPCADMLine);
    plothistLeft->getYAxis()->setDrawMode2(JKQTPCADMLineTicks);
    plothistLeft->setGrid(false);
    plothistBottom->synchronizeXToMaster(plothist);
    plothistBottom->setAbsoluteY(0,1);
    plothistBottom->getYAxis()->setInverted(true);
    plothistBottom->getXAxis()->setShowZeroAxis(false);
    plothistBottom->getYAxis()->setShowZeroAxis(false);
    plothistBottom->getXAxis()->setDrawMode1(JKQTPCADMLine);
    plothistBottom->getXAxis()->setDrawMode2(JKQTPCADMLineTicks);
    plothistBottom->setGrid(false);
    plothistBottom->setMousePositionShown(false);

    lay->setColumnStretch(0,1);
    lay->setColumnStretch(1,3);
    lay->setRowStretch(0,3);
    lay->setRowStretch(1,1);

    size_t randomdatacolx_small=datastore1->addColumn("random data, x");
    size_t randomdatacoly_small=datastore1->addColumn("random data, y");
    size_t randomdatacoldist_small=datastore1->addColumn("random data, distribution/class");
    size_t randomdatacolx=datastore1->addColumn("random data, x");
    size_t randomdatacoly=datastore1->addColumn("random data, y");
    size_t randomdatacoldist=datastore1->addColumn("random data, distribution/class");

    std::random_device rd;
    std::mt19937 gen{rd()};
    gen.seed(12345);
    std::uniform_int_distribution<> ddecide(0,2);
    std::normal_distribution<> d1x{5,3};
    std::normal_distribution<> d1y{5,1};
    std::normal_distribution<> d2x{10,2};
    std::normal_distribution<> d2y{10,5};
    for (size_t i=0; i<500; i++) {
        double rx=0,ry=0;
        const int decide=ddecide(gen);
        if (decide==0) {
            rx=d1x(gen);
            ry=d1y(gen);
        } else {
            rx=d2x(gen);
            ry=d2y(gen);
        }
        if (i<150) {
            datastore1->appendToColumn(randomdatacolx_small, rx);
            datastore1->appendToColumn(randomdatacoly_small, ry);
            datastore1->appendToColumn(randomdatacoldist_small, std::min(1,decide));
        }
        datastore1->appendToColumn(randomdatacolx, rx);
        datastore1->appendToColumn(randomdatacoly, ry);
        datastore1->appendToColumn(randomdatacoldist, std::min(1,decide));
    }

    // 2.2. To visualize the data, a simple JKQTPPeakStreamGraph is used:
    JKQTPXYParametrizedScatterGraph* gDataHist;
    plothist->addGraph(gDataHist=new JKQTPXYParametrizedScatterGraph(plothist));
    gDataHist->setXYColumns(randomdatacolx,randomdatacoly);
    gDataHist->setSymbolColumn(randomdatacoldist);
    QMap<double, JKQTPGraphSymbols> mapped;
    mapped[0]=JKQTPGraphSymbols::JKQTPCross;
    mapped[1]=JKQTPGraphSymbols::JKQTPPlus;
    gDataHist->setMappedSymbolColumnFunctor(mapped);
    gDataHist->setSymbolSize(5);
    gDataHist->setSymbolColor(QColorWithAlphaF(QColor("red"), 0.7));
    gDataHist->setDrawLine(false);
    gDataHist->setTitle(QString("random data, $N="+QString::number(datastore1->getRows(randomdatacoldist))+"$"));


    // 3. Marginal (1D) Statistics of the x-position and y-position deistributions:
    // 3.1. First we calculate the x/y marginal histograms, as desribed in https://github.com/jkriege2/JKQtPlotter/tree/master/examples/datastore_statistics
    timer.start();
    jkqtpstatAddHHistogram1DAutoranged(plothistBottom->getPlotter(),  datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), 1.0, true);
    jkqtpstatAddVHistogram1DAutoranged(plothistLeft->getPlotter(),  datastore1->begin(randomdatacoly), datastore1->end(randomdatacoly), 1.0, true);
    qDebug()<<"histogram, 1D: "<<timer.elapsed()/2.0<<"ms";

    // 4. 2D Histogram
    double xmin=0, xmax=0;
    double ymin=0, ymax=0;
    timer.start();
    jkqtpstatMinMax(datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), xmin,xmax);
    jkqtpstatMinMax(datastore1->begin(randomdatacoly), datastore1->end(randomdatacoly), ymin,ymax);
    size_t Nx=jkqtp_ceilTo<size_t>((xmax-xmin)/2.0);
    size_t Ny=jkqtp_ceilTo<size_t>((ymax-ymin)/2.0);
    size_t histcol=datastore1->addImageColumn(Nx, Ny, "2d histogram");
    jkqtpstatHistogram2D(datastore1->begin(randomdatacolx), datastore1->end(randomdatacolx), datastore1->begin(randomdatacoly), datastore1->end(randomdatacoly),
                         datastore1->begin(histcol),
                         xmin, xmax, ymin, ymax,
                         Nx, Ny, true);
    qDebug()<<"histogram, 2D: "<<timer.elapsed()<<"ms";
    JKQTPColumnMathImage* gHist;
    plothist->addGraph(gHist=new JKQTPColumnMathImage(plothist));
    gHist->setImageColumn(static_cast<int>(histcol));
    gHist->setX(xmin);
    gHist->setY(ymin);
    gHist->setWidth(xmax-xmin);
    gHist->setHeight(ymax-ymin);
    gHist->setTitle("2D Histogram");
    qDebug()<<xmin<<xmax<<Nx;
    qDebug()<<ymin<<ymax<<Ny;


    // autoscale the plot so the graph is contained
    plothist->zoomToFit();
    plothist->setGrid(false);
    plothist->getXAxis()->setShowZeroAxis(false);
    plothist->getYAxis()->setShowZeroAxis(false);
    plothist->getMainKey()->setBackgroundColor(QColorWithAlphaF("white", 0.25), Qt::SolidPattern);
    plothist->getPlotter()->moveGraphTop(gDataHist);
    plothistBottom->zoomToFit(false, true);
    plothistLeft->zoomToFit(true, false);

    // show plotter and make it a decent size
    mainWidget.show();
    mainWidget.resize(650,600);

    return QApplication::exec();
}
