#include <EPICSDataInterface.h>

EPICSDataInterface::EPICSDataInterface(QObject *parent) :
QObject(parent),
selectedCamera(CameraIndex_t::k_NOCAMERA),
provider(new pvac::ClientProvider("pva")),
channel(nullptr),
mon(nullptr) {

}

EPICSDataInterface::~EPICSDataInterface() {
    delete provider;
    delete channel;
}

void EPICSDataInterface::CameraConnectRequested() {
    channel = new pvac::ClientChannel(provider->connect(ImageSources[selectedCamera]));
    mon = new pvac::MonitorSync(channel->monitor());
    emit CameraPVSubscriptionCompleted();
}

void EPICSDataInterface::SelectedCameraChanged(int index) {
    selectedCamera = static_cast<CameraIndex_t>(index);
    std::cout << "Selected Camera: " << ImageSources[selectedCamera] << std::endl;
}

void EPICSDataInterface::UpdateImage() {
    // FIXME:
    // Use get here instead of monitor
    while (!mon->wait()) continue;

    switch (mon->event.event) {
        // Subscription network/internal error
        case pvac::MonitorEvent::Fail:
            std::cerr << mon->name() << " : Error : " << mon->event.message << "\n";
            break;
            // explicit call of 'mon.cancel' or subscription dropped
        case pvac::MonitorEvent::Cancel:
            std::cout << mon->name() << " <Cancel>\n";
            break;
            // Underlying channel becomes disconnected
        case pvac::MonitorEvent::Disconnect:
            std::cout << mon->name() << " <Disconnect>\n";
            break;
            // Data queue becomes not-empty
        case pvac::MonitorEvent::Data:
            // We drain event FIFO completely
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
                                    std::cout << data.size() << " bytes\n";
                                }
                            }
                        }
                    }
                }
            }
            // check to see if more events might be sent
            if (mon->complete()) {
                std::cout << mon->name() << " : <Complete>\n";
            }
            break;
    }
}


