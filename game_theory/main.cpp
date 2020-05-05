#include <QApplication>
#include "gamesolver.h"
#include "computeframe.h"
#include "simplex.h"

int main(int argc, char *argv[])
{
    QApplication ap(argc, argv);
    ComputeFrame w;
    w.show();
    return ap.exec();
}
