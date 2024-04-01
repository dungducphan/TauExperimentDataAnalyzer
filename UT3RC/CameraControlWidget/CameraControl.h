#pragma once

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class CameraControl; }
QT_END_NAMESPACE

class CameraControl : public QWidget {
Q_OBJECT

public:
    explicit CameraControl(QWidget *parent = nullptr);

    ~CameraControl() override;

private:
    Ui::CameraControl *ui;
};
