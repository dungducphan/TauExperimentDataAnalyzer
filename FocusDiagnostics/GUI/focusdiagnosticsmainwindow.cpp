#include "focusdiagnosticsmainwindow.h"
#include "FocusDiagnostics_autogen/include/ui_focusdiagnosticsmainwindow.h"

FocusDiagnosticsMainWindow::FocusDiagnosticsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FocusDiagnosticsMainWindow) {
    ui->setupUi(this);

    InitializeUI();
    UIFollowModeChanged();
}

FocusDiagnosticsMainWindow::~FocusDiagnosticsMainWindow() {
    delete ui;
}
void FocusDiagnosticsMainWindow::InitializeUI() {
    ui->ComboBox_MODE->setCurrentText("Online Monitoring");
    ui->stackedWidget->setCurrentWidget(ui->page_ONLINE_MONITORING);
}

void FocusDiagnosticsMainWindow::UIFollowModeChanged() {
    connect(ui->ComboBox_MODE, &QComboBox::currentTextChanged, [this](const QString &text) {
        if (text == "Online Monitoring") {
            ui->stackedWidget->setCurrentWidget(ui->page_ONLINE_MONITORING);
        } else if (text == "Offline Analysis") {
            ui->stackedWidget->setCurrentWidget(ui->page_OFFLINE_ANALYSIS);
        }
    });
}
