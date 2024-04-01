#pragma once

#include <QWidget>

#include <jkqtplotter.h>

QT_BEGIN_NAMESPACE
namespace Ui { class CameraControl; }
QT_END_NAMESPACE

class CameraControl : public QWidget {
Q_OBJECT

public:
    explicit CameraControl(QWidget *parent = nullptr);
    ~CameraControl() override;

    Ui::CameraControl* GetUI() { return ui; }
    JKQTPlotter* GetImagePlotter() { return imagePlotter; }

private:
    Ui::CameraControl *ui;
    JKQTPlotter* imagePlotter;
};
