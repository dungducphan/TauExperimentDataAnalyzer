#include <QApplication>

#include <focusdiagnosticsmainwindow.h>

#include <iostream>

#include <pva/client.h>
#include <cadef.h>

char *connState[] = {"Never", "Previously", "Connected", "Closed"};

int main(int argc, char *argv[]) {

    chid chan;
    SEVCHK(ca_create_channel(argv[1], NULL, NULL, 0, &chan),
           "Create channel failed");
    SEVCHK(ca_pend_io(1.0), "CA Search failed");
    printf("PV: %s\n", ca_name(chan));
    printf(" State: %s\n", connState[ca_state(chan)]);
    printf(" Server: %s\n", ca_host_name(chan));
    printf(" Data type: %s\n", dbr_type_to_text(ca_field_type(chan)));
    printf(" Element count: %u\n", ca_element_count(chan));

//    QApplication a(argc, argv);
//
//    try {
//        if (argc <= 1) {
//            std::cerr << "Usage: " << argv[0] << " <pvname>\n";
//        }
//        pvac::ClientProvider provider("pva");
//        pvac::ClientChannel channel(provider.connect(argv[1]));
//        std::cout << channel.name() <<" : "<< channel.get() << "\n";
//    } catch (std::exception& e){
//        std::cerr << "Error: " << e.what() << "\n";
//    }
//
//    FocusDiagnosticsMainWindow w;
//    w.show();
//
//    return a.exec();
}
