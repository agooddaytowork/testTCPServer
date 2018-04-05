#include <QCoreApplication>
#include "fountainserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    fountainServer aServer;
    return a.exec();
}
