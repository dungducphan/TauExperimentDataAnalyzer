#pragma once

#include <functional>

#include <QApplication>
#include <QDir>

#include <jkqtplotter/jkqtplotter.h>

#include <TauAppSettingController.h>

class TauApplication: public QApplication {
    Q_OBJECT
public:
    TauApplication(int &argc, char **argv, bool waitForScreenshotReady=false);

    virtual ~TauApplication();
    void addExportStepFunctor(const std::function<void(void)>& f);
    void addExportStepPlot(JKQTPlotter* plot);
    void addExportStepPlotFunctor(const std::function<JKQTPlotter*(void)>& fplot);
    int exec();
    public Q_SLOTS:
        void notifyReadyForScreenshot();
protected:
    bool waitForScreenshotReady;
    bool readyForScreenshot;
    QDir screenshotDir;
    bool saveScreenshot;
    bool saveSmallScreenshot;
    bool saveScreenshotPlot;
    bool saveSmallScreenshotPlot;
    bool scaleDownFromHighDPI;
    bool iterateFunctorSteps;
    bool iterateFunctorStepsSupressInitial;
    QStringList screenshotBasename;
    struct Data {
        enum Type {
            FunctorType,
            PlotterType,
            PlotterFunctorType
        };
        Type type;
        std::function<void(void)> f;
        std::function<JKQTPlotter*(void)> plotf;
        JKQTPlotter* p;
        inline Data(const std::function<void(void)>& f_):
            type(FunctorType), f(f_), plotf(), p(nullptr)
        {}
        inline Data(JKQTPlotter* p_):
            type(PlotterType), f(), plotf(), p(p_)
        {}
        inline Data(std::function<JKQTPlotter*(void)> p_):
            type(PlotterFunctorType), f(), plotf(p_), p(nullptr)
        {}
    };

    QVector<Data> functors;
    void readCmdLine();
    QRect getBoundsWithoutColor(QImage qImage, const QColor &exclusionColor = Qt::white);

    void saveWidget(QWidget* w, int iVisible);
};
