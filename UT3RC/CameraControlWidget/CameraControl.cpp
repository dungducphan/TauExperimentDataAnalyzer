#include <CameraControl.h>
#include <ui_CameraControl.h>

CameraControl::CameraControl(QWidget *parent) :
        QWidget(parent), ui(new Ui::CameraControl),
        imagePlotter(nullptr) {
    ui->setupUi(this);
    imagePlotter = new JKQTPlotter(ui->widget_IMAGE);
}

CameraControl::~CameraControl() {
    delete ui;
}
