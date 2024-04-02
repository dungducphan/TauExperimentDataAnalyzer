#pragma once

#include <QMainWindow>

#include <jkqtplotter.h>
#include <graphs/jkqtpimage.h>

#include <epicsEvent.h>
#include <pva/client.h>

QT_BEGIN_NAMESPACE
namespace Ui { class RCMainWindow; }
QT_END_NAMESPACE

class RCMainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit RCMainWindow(QWidget *parent = nullptr);
    ~RCMainWindow() override;

    [[nodiscard]] Ui::RCMainWindow* GetUi() const { return ui; }

    public slots:
    void OnImageReceived(const epics::pvData::shared_vector<const uint8_t>& image, const unsigned int& monID, const unsigned int& imageCounter);

private:
    Ui::RCMainWindow *ui;

    JKQTPlotter* especPointingPlotter;
    JKQTPlotter* especSpectrumLEPlotter;
    JKQTPlotter* especSpectrumHEPlotter;
    JKQTPlotter* wfsIntensityPlotter;
    JKQTPlotter* wfsDensityPlotter;

private:
    void UIConfig();
    void UpdatePlotter(JKQTPlotter* plotter, const epics::pvData::shared_vector<const uint8_t>& image, const unsigned int &monID, const unsigned int& imageCounter);
};

