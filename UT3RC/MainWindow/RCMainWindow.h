//
// Created by ubuntu on 4/1/24.
//

#ifndef TAUEXPERIMENTDATAANALYZER_RCMAINWINDOW_H
#define TAUEXPERIMENTDATAANALYZER_RCMAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class RCMainWindow; }
QT_END_NAMESPACE

class RCMainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit RCMainWindow(QWidget *parent = nullptr);

    ~RCMainWindow() override;

private:
    Ui::RCMainWindow *ui;
};


#endif //TAUEXPERIMENTDATAANALYZER_RCMAINWINDOW_H
