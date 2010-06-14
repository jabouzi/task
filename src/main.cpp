#include <QApplication>
#include "taskManager.h"
//
int main(int argc, char ** argv)
{
    QApplication app( argc, argv );
    TaskManager win;
    win.show(); 
    app.setQuitOnLastWindowClosed(false);
    return app.exec();
}
