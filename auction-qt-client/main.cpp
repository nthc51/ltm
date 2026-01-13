#include <QApplication>
#include "windows/loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("Auction System");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("Auction Corp");
    
    LoginWindow loginWindow;
    loginWindow.show();
    
    return app.exec();
}
