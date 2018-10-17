#include "fenetresecondaire.h"



FenetreSecondaire::FenetreSecondaire(QString &code, QWidget *parent = 0 ) : QDialog(parent)
{
// CONFIGURATION DE LA FENETRE
    setWindowTitle("Zero Generator");



    m_sortieText = new QTextEdit();
    m_sortieText->setPlainText(code);
    m_sortieText->setReadOnly(true);
    m_sortieText->setFont(QFont("Courier"));
    m_sortieText->setLineWrapMode(QTextEdit::NoWrap);

    m_fermer = new QPushButton("Fermer") ;

    resize(350, 450);

    QVBoxLayout *sortieLayout = new QVBoxLayout ;
    sortieLayout->addWidget(m_sortieText);
    sortieLayout->addWidget(m_fermer);

    setLayout(sortieLayout);

    connect(m_fermer,SIGNAL(clicked(bool)),this,SLOT(accept()));


}
