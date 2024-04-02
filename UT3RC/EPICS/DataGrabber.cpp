#include <DataGrabber.h>

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
provider_WFS(new pvac::ClientProvider("pva")),
channel_WFS(nullptr),
mon_WFS(nullptr) {

}

DataGrabber::~DataGrabber() {
    delete provider_ESpecPointing;
    delete provider_ESpecSpectrumLE;
    delete provider_ESpecSpectrumHE;
    delete provider_WFS;
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

    // Connect to IOC for WFS
    channel_WFS = new pvac::ClientChannel(provider_WFS->connect("Plasma:Density:IntensityMap"));
    mon_WFS = new pvac::MonitorSync(channel_WFS->monitor());

    // Wait for all monitors to be connected
    sleep(1);

    std::cout << "IOCs connected." << std::endl;
}
