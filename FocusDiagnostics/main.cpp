// Qt6
#include <QApplication>
#include <QIcon>
#include <QTimer>

// Imaging Source SDK
#include <gst/gst.h>

// Custom
#include <focusdiagnosticsmainwindow.h>
#include <Controller.h>
#include <SpeedTestPlot.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/Resources/Icons/icons8-laser-48.png"));

//    SpeedTestPlot plot;
//    QTimer::singleShot(500, &plot, SLOT(plotNewData()));

    gst_debug_set_default_threshold(GST_LEVEL_WARNING);
    gst_init(nullptr, nullptr);

    auto controller = new Controller();
    controller->ShowView();

    return QApplication::exec();
}
