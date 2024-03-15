#ifndef FOCUSDIAGNOSTICSMAINWINDOW_H
#define FOCUSDIAGNOSTICSMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class FocusDiagnosticsMainWindow;
}

class FocusDiagnosticsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FocusDiagnosticsMainWindow(QWidget *parent = nullptr);
    ~FocusDiagnosticsMainWindow();

private:
    Ui::FocusDiagnosticsMainWindow *ui;
};

#endif // FOCUSDIAGNOSTICSMAINWINDOW_H
