//
// Created by ubuntu on 4/1/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RCMainWindow.h" resolved

#include "RCMainWindow.h"
#include "ui_RCMainWindow.h"


RCMainWindow::RCMainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::RCMainWindow) {
    ui->setupUi(this);
}

RCMainWindow::~RCMainWindow() {
    delete ui;
}
