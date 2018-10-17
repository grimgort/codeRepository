#ifndef FENTRE_PRINCIPAL_H
#define FENTRE_PRINCIPAL_H


#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QString>




class FenetrePrincpal : public QWidget
{
    Q_OBJECT

public:
    FenetrePrincpal();

private slots:
    void genererCode();

private:
    QGroupBox *m_inclusion;
    QCheckBox *m_constructeur;
    QCheckBox *m_destructeur;
    QGroupBox *m_commentaireCheck;
    QLineEdit *m_auteur;
    QDateEdit *m_date;
    QTextEdit *m_role;
    QLineEdit *m_nom;
    QLineEdit *m_classMere;
    QLineEdit *m_header;





};








































#endif // FENTRE_PRINCIPAL_H
