//
// Created by ubuntu on 4/1/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CameraControl.h" resolved

#include <CameraControl.h>
#include <ui_CameraControl.h>


CameraControl::CameraControl(QWidget *parent) :
        QWidget(parent), ui(new Ui::CameraControl) {
    ui->setupUi(this);
}

CameraControl::~CameraControl() {
    delete ui;
}
