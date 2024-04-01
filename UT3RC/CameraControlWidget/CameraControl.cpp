#include <CameraControl.h>
#include <ui_CameraControl.h>


CameraControl::CameraControl(QWidget *parent) :
        QWidget(parent), ui(new Ui::CameraControl) {
    ui->setupUi(this);
}

CameraControl::~CameraControl() {
    delete ui;
}
