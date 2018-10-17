#include <QApplication>
#include <QtWidgets>
#include "fenetreprincipal.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FenetrePrincipal fenetre;
    fenetre.show();

    return app.exec();
}
