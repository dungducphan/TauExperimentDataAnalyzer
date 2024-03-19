#include "CameraCoreGUI.h"
#include "LaserDiagnostics_autogen/include/ui_CameraCoreGUI.h"

CameraCoreGUI::CameraCoreGUI(QWidget *parent) :
        QWidget(parent), ui(new Ui::CameraCoreGUI) {
    ui->setupUi(this);
}

CameraCoreGUI::~CameraCoreGUI() {
    delete ui;
}
