//
// Created by ubuntu on 4/1/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RCMainWindow.h" resolved

#include "RCMainWindow.h"
#include "ui_RCMainWindow.h"


RCMainWindow::RCMainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::RCMainWindow),
        especPointingPlotter(new JKQTPlotter(this)),
        especSpectrumLEPlotter(new JKQTPlotter(this)),
        especSpectrumHEPlotter(new JKQTPlotter(this)),
        wfsIntensityPlotter(new JKQTPlotter(this)),
        wfsDensityPlotter(new JKQTPlotter(this)) {
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

    wfsIntensityPlotter->getPlotter()->setUseAntiAliasingForGraphs(false);
    wfsIntensityPlotter->getPlotter()->setUseAntiAliasingForSystem(false);
    wfsIntensityPlotter->getPlotter()->setUseAntiAliasingForText(false);
    wfsIntensityPlotter->setGrid(false);
    wfsIntensityPlotter->setMousePositionShown(false);

    wfsDensityPlotter->getPlotter()->setUseAntiAliasingForGraphs(false);
    wfsDensityPlotter->getPlotter()->setUseAntiAliasingForSystem(false);
    wfsDensityPlotter->getPlotter()->setUseAntiAliasingForText(false);
    wfsDensityPlotter->setGrid(false);
    wfsDensityPlotter->setMousePositionShown(false);

    // ATTACH TO ESPEC TAB
    especPointingPlotter->setParent(ui->tab_ESPEC);
    especSpectrumLEPlotter->setParent(ui->tab_ESPEC);
    especSpectrumHEPlotter->setParent(ui->tab_ESPEC);
    wfsIntensityPlotter->setParent(ui->tab_ESPEC);
    wfsDensityPlotter->setParent(ui->tab_ESPEC);

    // ESPEC TAB CONFIG
    ui->tab_ESPEC->setLayout(new QGridLayout());
    auto aLayout = (QGridLayout*) ui->tab_ESPEC->layout();
    aLayout->addWidget(especPointingPlotter, 0, 1);
    aLayout->addWidget(especSpectrumLEPlotter, 0, 2);
    aLayout->addWidget(especSpectrumHEPlotter, 1, 2);
    aLayout->addWidget(wfsIntensityPlotter, 0, 0);
    aLayout->addWidget(wfsDensityPlotter, 1, 0);
    aLayout->setColumnStretch(0, 2);
    aLayout->setColumnStretch(1, 2);
    aLayout->setColumnStretch(2, 5);
}
