#include "mafenetre.h"


MaFenetre::MaFenetre(int hauteur, int largeur) : QWidget()
{
    setFixedSize(hauteur, largeur);

    m_monBouton = new QPushButton("Salut les Zéros, la forme ?",this);  //boute qui sera inséré dans une fenêtre quelquonque

    m_monBouton -> setText("QUITTER");  //Permet de modifier le texte dans un bouton
    m_monBouton ->setToolTip("Fenêtre d'aide");    //permet d'afficher une info bulle en passant la souris sur le bouton.
    m_monBouton ->setFont(QFont("Comic Sans MS", 20, QFont::Bold, true));  //Définit la police de caractére avec la class QFont.
    m_monBouton ->setCursor(Qt::PointingHandCursor);   //défini l'image de la souris en passant sur le bouton
    m_monBouton ->setIcon(QIcon("smile.jpg")); //rajoute une icone
    m_monBouton ->setGeometry(60, 50, 180, 70); //déplace le bouton dans la fenêtre

    QObject::connect(m_monBouton,SIGNAL(clicked()),qApp,SLOT(quit())) ;

}



MaFenetre2::MaFenetre2() : QWidget()
{
    setFixedSize(300, 300);

    m_lcd = new QLCDNumber(this);
    m_lcd->setSegmentStyle(QLCDNumber::Flat);
    m_lcd->move(50, 20);

    m_progressBar = new QProgressBar (this);
//    m_progressBar->move(50, 50);
    m_progressBar->setGeometry(10, 50, 180, 20);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setGeometry(10, 80, 150, 20);

    QObject::connect(m_slider, SIGNAL(valueChanged(int)), m_lcd, SLOT(display(int))) ;
    QObject::connect(m_slider, SIGNAL(valueChanged(int)), m_progressBar, SLOT(setValue(int))) ;
    m_boutonReset = new QPushButton("Reset",this);
    m_lcd->move(70, 100);

    QObject::connect(m_boutonReset, SIGNAL(clicked()), m_progressBar, SLOT(reset())) ;

    m_slider->setRange(200, 600);
    QObject::connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(changerLargeur(int))) ;

    m_slider2 = new QSlider(Qt::Vertical, this);
    m_slider2->setGeometry(200, 10, 20, 150);
    m_slider2->setRange(300, 600);
    QObject::connect(m_slider2, SIGNAL(valueChanged(int)), this, SLOT(changerHauteur(int))) ;
    QObject::connect(this, SIGNAL(agrandissementMax()), qApp, SLOT(quit()));


}

void MaFenetre2::changerLargeur(int largeur)
{
    setFixedSize(largeur,height());
    if (largeur==600)
    {
        agrandissementMax();
    }
}

void MaFenetre2::changerHauteur(int hauteur)
{
    setFixedSize(width(),hauteur);
}


MaFenetre3::MaFenetre3() : QWidget()
{
    setFixedSize(230, 120);

    m_boutonDialogue = new QPushButton("Ouvrir la boîte de dialogue", this);
    m_boutonDialogue->move(40, 50);

    QObject::connect(m_boutonDialogue, SIGNAL(clicked()), this, SLOT(ouvrirDialogue()));
}

void MaFenetre3::ouvrirDialogue()
{
    int reponse = QMessageBox::question(this, "Interrogatoire", "Dites voir, je me posais la question comme cela, êtes-vous vraiment un Zéro ?", QMessageBox ::Yes | QMessageBox::No);
    if (reponse==QMessageBox::Yes)
    {
        QMessageBox::information(this,"Interrogatoire","Vous suivez la bonne voie") ;
    }
    else if (reponse==QMessageBox::No)
    {
        QMessageBox::critical(this,"Interrogatoire","Vous suivez la voie du coté obscure") ;
    }


}


MaFenetre4::MaFenetre4() : QWidget()
{
    setFixedSize(230, 120);

    m_boutonDialogue2 = new QPushButton("Ouvrir la boîte de dialogue", this);
    m_boutonDialogue2->move(40, 50);

    QObject::connect(m_boutonDialogue2, SIGNAL(clicked()), this, SLOT(ouvrirDialogue()));
}


void MaFenetre4::ouvrirDialogue()
{
    bool ok = false;
    QString pseudo = QInputDialog::getText(this, "Pseudo", "Quel est votre pseudo ?", QLineEdit::Normal, QString(), &ok);

    if (ok && !pseudo.isEmpty())
    {
        QMessageBox::information(this, "Pseudo", "Bonjour " + pseudo + ", ça va ?");
        m_boutonDialogue2 -> setText(pseudo);
    }
    else
    {
        QMessageBox::critical(this, "Pseudo", "Vous n'avez pas voulu donner votre nom… snif.");
    }
}



MaFenetre5::MaFenetre5() : QWidget()
{
    setFixedSize(230, 120);

    m_boutonDialogue2 = new QPushButton("Ouvrir la boîte de dialogue", this);
    m_boutonDialogue2->move(40, 50);
    m_boutonDialogue3 = new QPushButton("Ouvrir la boîte de dialogue fichier", this);
    m_boutonDialogue3->move(20, 20);


    QObject::connect(m_boutonDialogue2, SIGNAL(clicked()), this, SLOT(ouvrirDialogue()));
    QObject::connect(m_boutonDialogue3, SIGNAL(clicked()), this, SLOT(ouvrirDossier()));
}


void MaFenetre5::ouvrirDialogue()
{

    QColor couleur = QColorDialog::getColor(Qt::white, this);

    QPalette palette;
    palette.setColor(QPalette::ButtonText, couleur);
    m_boutonDialogue2->setPalette(palette);


}

void MaFenetre5::ouvrirDossier()
{
    QString dossier = QFileDialog::getExistingDirectory(this);
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    QMessageBox::information(this, "Fichier", "Vous avez sélectionné :\n" + fichier);
    QString fichier2 = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");

}



