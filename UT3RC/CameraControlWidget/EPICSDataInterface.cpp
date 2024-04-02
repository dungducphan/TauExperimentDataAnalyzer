#include <EPICSDataInterface.h>

EPICSDataInterface::EPICSDataInterface(QObject *parent) :
QObject(parent),
selectedCamera(CameraIndex_t::k_NOCAMERA),
provider(new pvac::ClientProvider("pva")),
channel(nullptr),
mon(nullptr),
monitorThread(nullptr),
isMonitoring(false),
imageCounter(0) {
}

EPICSDataInterface::~EPICSDataInterface() {
    delete provider;
    delete channel;
    delete monitorThread;
}

void EPICSDataInterface::OnCameraConnectRequested() {
    channel = new pvac::ClientChannel(provider->connect(ImageSources[selectedCamera]));
    mon = new pvac::MonitorSync(channel->monitor());

    isMonitoring = true;
    delete monitorThread;
    monitorThread = new std::thread(&EPICSDataInterface::MonitorPV, this);
    monitorThread->detach();
    emit MonitorThreadStarted();
}

void EPICSDataInterface::OnSelectedCameraChanged(int index) {
    selectedCamera = static_cast<CameraIndex_t>(index);
    std::cout << "Selected Camera: " << ImageSources[selectedCamera] << std::endl;
}

void EPICSDataInterface::MonitorPV() {
    while (isMonitoring) {
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
                                    epics::pvData::shared_vector<const uint8_t> data = valueArray->view();
                                    if (!data.empty()) {
                                        imageCounter++;
                                        emit ImageReceived(data, imageCounter);
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
    emit MonitorThreadFinished();
}

void EPICSDataInterface::OnCameraDisconnectRequested() {
    isMonitoring = false;
}


