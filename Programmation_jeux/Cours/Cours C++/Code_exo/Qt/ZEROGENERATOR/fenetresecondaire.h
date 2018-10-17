#ifndef FENETRESECONDAIRE_H
#define FENETRESECONDAIRE_H

#include <QTextEdit>
#include <QDialog>
#include <QFormLayout>
#include <QPushButton>

class FenetreSecondaire : public QDialog
{
public:
    FenetreSecondaire(QString &code,QWidget *parent);


private:
    QTextEdit *m_sortieText;
    QPushButton *m_fermer ;

};

#endif // FENETRESECONDAIRE_H
