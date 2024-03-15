#pragma once

#include <QMainWindow>

namespace Ui {
    class FocusDiagnosticsMainWindow;
}

class FocusDiagnosticsMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit FocusDiagnosticsMainWindow(QWidget *parent = nullptr);
    ~FocusDiagnosticsMainWindow() override;

private:
    Ui::FocusDiagnosticsMainWindow *ui;

    void InitializeUI();
    void UIFollowModeChanged();
};
