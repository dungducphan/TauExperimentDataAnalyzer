#include <QApplication>
#include <focusdiagnosticsmainwindow.h>

#include <iostream>
#include <pva/client.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    try {
        if (argc <= 1) {
            std::cerr << "Usage: " << argv[0] << " <pvname>\n";
        }
        pvac::ClientProvider provider("pva");
        pvac::ClientChannel channel(provider.connect(argv[1]));
        std::cout << channel.name() <<" : "<< channel.get() << "\n";
    } catch (std::exception& e){
        std::cerr << "Error: " << e.what() << "\n";
    }

    FocusDiagnosticsMainWindow w;
    w.show();

    return a.exec();
}
