/*
#include <QApplication> // inclue toute les bibliotheque classique(iostream,vector,etc...)
#include <QPushButton> // permet d'afficher des boutons


int main(int argc, char *argv[])
{

    QApplication app(argc, argv);   //option de compilation de Qt


     // Dans Qt, accéder à la valeur d'un attribut est la méthode portant le nom de l'attribut (accesseur).
     // Pour modifier cet attribut, c'est la méthode portant un set avant le nom de l'acceseur

    QWidget fenetre;
    fenetre.setFixedSize(300, 150);

    QPushButton bouton("Salut les Zéros, la forme ?",&fenetre);  //boute qui sera inséré dans une fenêtre quelquonque

    bouton.setText("J'ai changé mon bouton");  //Permet de modifier le texte dans un bouton
    bouton.setToolTip("Fenêtre d'aide");    //permet d'afficher une info bulle en passant la souris sur le bouton.
    bouton.setFont(QFont("Comic Sans MS", 20, QFont::Bold, true));  //Définit la police de caractére avec la class QFont.
    bouton.setCursor(Qt::PointingHandCursor);   //défini l'image de la souris en passant sur le bouton
    bouton.setIcon(QIcon("smile.jpg")); //rajoute une icone
    bouton.setGeometry(60, 50, 180, 70); //déplace le bouton dans la fenêtre

    QPushButton bouton2("je m'incruste",&bouton);
    bouton2.move(30, 15);

//    bouton.show();  //montre le bouton dans la fenêtre
    fenetre.show(); // affiche la fenêtre et on contenant


    return app.exec();  //fin de l'execution de l'application

}
*/



#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "MaFenetre.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    MaFenetre fenetre;
//    fenetre.show();
//    MaFenetre fenetre2(500,400);
//    fenetre2.show();

//    MaFenetre2 fenetre2;
//    fenetre2.show();

//  Fenetre3 traduction yes no en oui non
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

//    MaFenetre3 fenetre3;
//    fenetre3.show();

    MaFenetre4 fenetre4;
    fenetre4.show();

     MaFenetre5 fenetre5;
     fenetre5.show();


    return app.exec();
}
