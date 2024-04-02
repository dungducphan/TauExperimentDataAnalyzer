#include "DataGrabber.h"

DataGrabber::DataGrabber(QObject *parent) :
        QObject(parent),
        provider_ESpecPointing(new pvac::ClientProvider("pva")),
        channel_ESpecPointing(nullptr),
        mon_ESpecPointing(nullptr),
        provider_ESpecSpectrumLE(new pvac::ClientProvider("pva")),
        channel_ESpecSpectrumLE(nullptr),
        mon_ESpecSpectrumLE(nullptr),
        provider_ESpecSpectrumHE(new pvac::ClientProvider("pva")),
        channel_ESpecSpectrumHE(nullptr),
        mon_ESpecSpectrumHE(nullptr),
        provider_WFSIntensity(new pvac::ClientProvider("pva")),
        channel_WFSIntensity(nullptr),
        mon_WFSIntensity(nullptr),
        provider_WFSDensity(new pvac::ClientProvider("pva")),
        channel_WFSDensity(nullptr),
        mon_WFSDensity(nullptr),
        exiting(false),
        monitorThread_ESpecPointing(nullptr),
        monitorThread_ESpecSpectrumLE(nullptr),
        monitorThread_ESpecSpectrumHE(nullptr),
        monitorThread_WFSIntensity(nullptr),
        monitorThread_WFSDensity(nullptr),
        shotCounter(0) {
}

DataGrabber::~DataGrabber() {
    exiting = true;
    std::cout << "Waiting for monitor threads to finish..." << std::endl;
    sleep(5);
    delete provider_ESpecPointing;
    delete provider_ESpecSpectrumLE;
    delete provider_ESpecSpectrumHE;
    delete provider_WFSIntensity;
    delete provider_WFSDensity;
    delete channel_ESpecPointing;
    delete channel_ESpecSpectrumLE;
    delete channel_ESpecSpectrumHE;
    delete channel_WFSIntensity;
    delete channel_WFSDensity;
    delete mon_ESpecPointing;
    delete mon_ESpecSpectrumLE;
    delete mon_ESpecSpectrumHE;
    delete mon_WFSIntensity;
    delete mon_WFSDensity;
    delete monitorThread_ESpecPointing;
    delete monitorThread_ESpecSpectrumLE;
    delete monitorThread_ESpecSpectrumHE;
    delete monitorThread_WFSIntensity;
    delete monitorThread_WFSDensity;
    std::cout << "DataGrabber destroyed." << std::endl;
}

void DataGrabber::ConnectToIOCs() {
    std::cout << "Connecting to IOCs..." << std::endl;

    // Connect to IOC for ESpecPointing
    channel_ESpecPointing = new pvac::ClientChannel(provider_ESpecPointing->connect("Electrons:Spectrometer:Pointing:PVA:Image"));
    mon_ESpecPointing = new pvac::MonitorSync(channel_ESpecPointing->monitor());

    // Connect to IOC for ESpecSpectrumLE
    channel_ESpecSpectrumLE = new pvac::ClientChannel(provider_ESpecSpectrumLE->connect("Electrons:Spectrometer:LowEnergy:PVA:Image"));
    mon_ESpecSpectrumLE = new pvac::MonitorSync(channel_ESpecSpectrumLE->monitor());

    // Connect to IOC for ESpecSpectrumHE
    channel_ESpecSpectrumHE = new pvac::ClientChannel(provider_ESpecSpectrumHE->connect("Electrons:Spectrometer:HighEnergy:PVA:Image"));
    mon_ESpecSpectrumHE = new pvac::MonitorSync(channel_ESpecSpectrumHE->monitor());

//    // Connect to IOC for WFS Intensity map
//    channel_WFSIntensity = new pvac::ClientChannel(provider_WFSIntensity->connect("Plasma:Density:IntensityMap"));
//    mon_WFSIntensity = new pvac::MonitorSync(channel_WFSIntensity->monitor());
//
//    // Connect to IOC for WFS Density map
//    channel_WFSDensity = new pvac::ClientChannel(provider_WFSDensity->connect("Plasma:Density:DensityMap"));
//    mon_WFSDensity = new pvac::MonitorSync(channel_WFSDensity->monitor());

    // Wait for all monitors to be connected
    sleep(1);

    std::cout << "IOCs connected." << std::endl;

    StartMonitoringThreads();
}

void DataGrabber::StartMonitoringThreads() {
    delete monitorThread_ESpecPointing;
    monitorThread_ESpecPointing = new std::thread(&DataGrabber::StartMonitoringThread, this, mon_ESpecPointing, 0);
    monitorThread_ESpecPointing->detach();

    delete monitorThread_ESpecSpectrumLE;
    monitorThread_ESpecSpectrumLE = new std::thread(&DataGrabber::StartMonitoringThread, this, mon_ESpecSpectrumLE, 1);
    monitorThread_ESpecSpectrumLE->detach();

    delete monitorThread_ESpecSpectrumHE;
    monitorThread_ESpecSpectrumHE = new std::thread(&DataGrabber::StartMonitoringThread, this, mon_ESpecSpectrumHE, 2);
    monitorThread_ESpecSpectrumHE->detach();

//    delete monitorThread_WFSIntensity;
//    monitorThread_WFSIntensity = new std::thread(&DataGrabber::StartMonitoringThread, this, mon_WFSIntensity, 3);
//    monitorThread_WFSIntensity->detach();
//
//    delete monitorThread_WFSDensity;
//    monitorThread_WFSDensity = new std::thread(&DataGrabber::StartMonitoringThread, this, mon_WFSDensity, 4);
//    monitorThread_WFSDensity->detach();
}

void DataGrabber::StartMonitoringThread(pvac::MonitorSync* mon, unsigned int monID) {
    std::cout << "Monitor Thread Started: " << mon->name() << "\n";
    while (!exiting) {
        if (!mon->wait()) continue;

        switch (mon->event.event) {
            case pvac::MonitorEvent::Fail:
                std::cerr << mon->name() << " : Error : " << mon->event.message << "\n";
                break;
            case pvac::MonitorEvent::Cancel:
                std::cout << mon->name() << " <Cancel>\n";
                break;
            case pvac::MonitorEvent::Disconnect:
                std::cout << mon->name() << " <Disconnect>\n";
                break;
            case pvac::MonitorEvent::Data:
                while (mon->poll()) {
                    if (mon->root) {
                        std::shared_ptr<const epics::pvData::PVField> valueField = mon->root->getSubField("value");
                        if (valueField) {
                            std::shared_ptr<const epics::pvData::PVUnion> constValueUnion = std::dynamic_pointer_cast<const epics::pvData::PVUnion>(
                                    valueField);
                            if (constValueUnion) {
                                epics::pvData::PVFieldPtr nonConstField = std::const_pointer_cast<epics::pvData::PVField>(
                                        constValueUnion->get());
                                epics::pvData::PVUByteArrayPtr valueArray = std::dynamic_pointer_cast<epics::pvData::PVUByteArray>(
                                        nonConstField);
                                if (valueArray) {
                                    shotCounter++;
                                    epics::pvData::shared_vector<const uint8_t> data = valueArray->view();
                                    if (!data.empty()) {
                                        emit ImageReceived(data, monID, shotCounter);
                                    }
                                }
                            }
                        }
                    }
                }
                if (mon->complete()) {
                    std::cout << mon->name() << " : <Complete>\n";
                }
                break;
        }
    }
    std::cout << "Monitor Thread Finished\n";
}
