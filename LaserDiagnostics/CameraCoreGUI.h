#pragma once

#include <QWidget>

namespace Ui { class CameraCoreGUI; }

class CameraCoreGUI : public QWidget {
Q_OBJECT

public:
    explicit CameraCoreGUI(QWidget *parent = nullptr);

    ~CameraCoreGUI() override;

private:
    Ui::CameraCoreGUI *ui;
};

