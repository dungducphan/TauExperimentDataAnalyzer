#pragma once

#include <QObject>

#include <epicsEvent.h>
#include <pva/client.h>

class DataGrabber : public QObject {
    Q_OBJECT
public:
    explicit DataGrabber(QObject* parent = nullptr);
    ~DataGrabber() override;

    void ConnectToIOCs();

private:
    pvac::ClientProvider* provider_ESpecPointing;
    pvac::ClientChannel* channel_ESpecPointing;
    pvac::MonitorSync* mon_ESpecPointing;

    pvac::ClientProvider* provider_ESpecSpectrumLE;
    pvac::ClientChannel* channel_ESpecSpectrumLE;
    pvac::MonitorSync* mon_ESpecSpectrumLE;

    pvac::ClientProvider* provider_ESpecSpectrumHE;
    pvac::ClientChannel* channel_ESpecSpectrumHE;
    pvac::MonitorSync* mon_ESpecSpectrumHE;

    pvac::ClientProvider* provider_WFS;
    pvac::ClientChannel* channel_WFS;
    pvac::MonitorSync* mon_WFS;
};
