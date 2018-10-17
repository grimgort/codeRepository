#include "fentre_principal.h"
#include "fenetresecondaire.h"



FenetrePrincpal::FenetrePrincpal() : QWidget()
{
// CONFIGURATION DE LA FENETRE
    setWindowTitle("Zero Generator");
    setWindowIcon(QIcon ("zero.jpg")) ;
    //setFixedSize(400,800);

// CONFIGURATION DU FORMULAIRE
    m_nom = new QLineEdit ;
    m_classMere = new QLineEdit ;

    QGroupBox *formulaireLabel = new QGroupBox("Définition de la classe",this);

    QFormLayout *formulaireNom = new QFormLayout;
    formulaireNom->addRow("Nom de la classe : ", m_nom);
    formulaireNom->addRow("Nom de la classe mère : ", m_classMere);


    formulaireLabel->setLayout(formulaireNom);




// CONFIGURATION DES OPTIONS


    m_inclusion = new QGroupBox("Protéger le header contre les inclusions multiples");
    m_inclusion->setCheckable(true);
    m_inclusion->setChecked(true);
    m_header = new QLineEdit;
//    m_header->setPlaceholderText("toto");
    QFormLayout *inclusionLayout = new QFormLayout ;
    inclusionLayout->addRow("Header",m_header);
    m_inclusion->setLayout(inclusionLayout);
    connect(m_nom, SIGNAL(textEdited(QString)), m_header, SLOT(setText(QString)));


    m_constructeur = new QCheckBox("Générer un constructeur par défault") ;
    m_constructeur->setChecked(true);
    m_destructeur = new QCheckBox("Générer un destructeur") ;
    m_destructeur->setChecked(true);
    QGroupBox *options = new QGroupBox("Options",this) ;
    QVBoxLayout *optionsLabel = new QVBoxLayout;
    optionsLabel->addWidget(m_inclusion);
    optionsLabel->addWidget(m_constructeur);
    optionsLabel->addWidget(m_destructeur);

    options -> setLayout(optionsLabel);


// CONFIGURATION DES COMMENTAIRES
    m_commentaireCheck = new QGroupBox("Ajouter un commentaire");
    m_commentaireCheck->setCheckable(true);
    m_commentaireCheck->setChecked(false);

    m_auteur = new QLineEdit ;
    m_date = new QDateEdit ;
    m_date->setDate(QDate::currentDate());
    m_role = new QTextEdit ;


    QFormLayout *commentaireLayout = new QFormLayout ;
    commentaireLayout->addRow("Auteur",m_auteur);
    commentaireLayout->addRow("Date de création",m_date);
    commentaireLayout->addRow("Rôle de la classe",m_role);


    m_commentaireCheck->setLayout(commentaireLayout);

// BOUTON DE SORTIE
    QPushButton *generer = new QPushButton("Générer");
    QPushButton *quitter = new QPushButton("Quitter");
    QHBoxLayout *boutonSortie = new QHBoxLayout;
    boutonSortie -> addWidget(generer);
    boutonSortie -> addWidget(quitter);
    boutonSortie -> setAlignment(Qt::AlignRight);

    connect(quitter, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(generer, SIGNAL(clicked()), this, SLOT(genererCode()));



// LAYOUT PRINCIPAL
    QVBoxLayout *positionPrincipal = new QVBoxLayout ;
    positionPrincipal->addWidget(formulaireLabel);
    positionPrincipal->addWidget(options);
    positionPrincipal->addWidget(m_commentaireCheck);
    positionPrincipal->addLayout(boutonSortie);

    setLayout(positionPrincipal);





}


void FenetrePrincpal::genererCode()
{


    if (m_nom->text().isEmpty())
    {
        QMessageBox::critical(this,"Erreur","Il faut rentrer un nom de class");
        return;
    }

    QString code;

    if (m_commentaireCheck->isChecked())
    {
        code+="/* \n";
        code+="Auteur : " + m_auteur->text() + "\n";
        code+="Date : " + m_date->text() + "\n";
        code+="Rôle : \n " ;
        code+=m_role->toPlainText() + "\n" ;
        code+="*/ \n\n\n";
    }

    if (m_inclusion->isChecked())
    {
        code+="#ifndef HEADER_" + m_header->text().toUpper() + "\n";
        code+="#define HEADER_" + m_header->text().toUpper() + "\n\n\n";
    }

    code+="class " + m_nom->text();
    if (!m_classMere->text().isEmpty())
    {
        code+=" : public " + m_classMere->text() ;
    }
    code+="\n{ \n       public: \n";

    if (m_constructeur->isChecked())
    {
        code+="              " + m_nom->text() + "(); \n";
    }

    if (m_destructeur->isChecked())
    {
        code+="              ~" + m_nom->text() + "(); \n";
    }

    code+="\n       protected: \n\n       private: \n\n\n};" ;


    if (m_inclusion->isChecked())
    {
        code+="\n\n\n\n#endif //  HEADER_" + m_nom->text().toUpper();
    }

    FenetreSecondaire *fenetreSecondaire = new FenetreSecondaire(code, this) ;
    fenetreSecondaire->exec();
}















