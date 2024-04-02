#pragma once

#include <thread>
#include <string>

#include <QObject>

#include <epicsEvent.h>
#include <pva/client.h>

class DataGrabber : public QObject {
    Q_OBJECT
public:
    explicit DataGrabber(QObject* parent = nullptr);
    ~DataGrabber() override;

    void ConnectToIOCs();

signals:
    void ImageReceived(const epics::pvData::shared_vector<const uint8_t>& image, const unsigned int& mon_ID, const unsigned int& imageCounter);

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

    pvac::ClientProvider* provider_WFSIntensity;
    pvac::ClientChannel* channel_WFSIntensity;
    pvac::MonitorSync* mon_WFSIntensity;

    pvac::ClientProvider* provider_WFSDensity;
    pvac::ClientChannel* channel_WFSDensity;
    pvac::MonitorSync* mon_WFSDensity;

    void StartMonitoringThreads();
    void StartMonitoringThread(pvac::MonitorSync*, unsigned int);
    std::thread* monitorThread_ESpecPointing;
    std::thread* monitorThread_ESpecSpectrumLE;
    std::thread* monitorThread_ESpecSpectrumHE;
    std::thread* monitorThread_WFSIntensity;
    std::thread* monitorThread_WFSDensity;
    bool exiting;
    unsigned int shotCounter;
};
