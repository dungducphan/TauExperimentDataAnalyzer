#include "focusdiagnosticsmainwindow.h"
#include "ui_focusdiagnosticsmainwindow.h"

FocusDiagnosticsMainWindow::FocusDiagnosticsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FocusDiagnosticsMainWindow)
{
    ui->setupUi(this);
}

FocusDiagnosticsMainWindow::~FocusDiagnosticsMainWindow()
{
    delete ui;
}
