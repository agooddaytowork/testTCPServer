#include <QCoreApplication>
#include "fountainserver.h"
#include "simpleserialinterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    fountainServer aServer;
    return a.exec();
}
