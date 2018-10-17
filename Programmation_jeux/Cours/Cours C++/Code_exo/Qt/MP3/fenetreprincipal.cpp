#include "fenetreprincipal.h"


FenetrePrincipal::FenetrePrincipal()
{
    // ON DEFINIT LA ZONE CENTRAL
    QMdiArea *zoneCentrale = new QMdiArea;
    setCentralWidget(zoneCentrale);

    // ON AJOUTE LA FENETRE CENTRAL
    QTabWidget *onglets = new QTabWidget  ;
    QMdiSubWindow *sousFenetre1 = zoneCentrale->addSubWindow(onglets,Qt::FramelessWindowHint);
    onglets->showMaximized() ;

    //ON REMPLIT LES PAGES DU WIDGET CENTRAL
    QWidget *page1 = new QWidget;
    QWidget *page2 = new QWidget;
    QWidget *page3 = new QWidget;
    onglets->addTab(page1, "Transfére");
    onglets->addTab(page2, "Liste de lecture");
    onglets->addTab(page3, "Liste des musics");

    // ON AJOUTE LE MENU
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
    QMenu *menuEdition = menuBar()->addMenu("&Edition");
    QMenu *menuAffichage = menuBar()->addMenu("&Affichage");



    // ON FABRIQUE PAGE2
    QPushButton *selectDossier = new QPushButton("Selection dossier") ;
    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(selectDossier);
    page2->setLayout(vbox2);
    connect(selectDossier, SIGNAL(clicked()), this, SLOT(ouvrirDossier()));





}


void FenetrePrincipal::ouvrirDossier()
{

     QString dossier = QFileDialog::getExistingDirectory(this);
    QMessageBox::information(this, "Fichier","coucou");
}

