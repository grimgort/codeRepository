#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QSlider>
#include <QProgressBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>


class MaFenetre : public QWidget
{
public:
    MaFenetre(int hauteur = 300, int largeur = 250);

private:
    QPushButton *m_monBouton;
};


class MaFenetre2 : public QWidget
{
    Q_OBJECT

public:
    MaFenetre2();
public slots:
    void changerLargeur(int largeur);
    void changerHauteur(int hauteur);
signals:
    void agrandissementMax();

private:
    QLCDNumber *m_lcd;
    QSlider *m_slider;
    QProgressBar *m_progressBar;
    QPushButton *m_boutonReset;
    QSlider *m_slider2;
};

class MaFenetre3 : public QWidget
{
    Q_OBJECT

public:
    MaFenetre3();
public slots:
    void ouvrirDialogue();

private:
    QPushButton *m_boutonDialogue;

};


class MaFenetre4 : public QWidget
{
    Q_OBJECT

public:
    MaFenetre4();
public slots:
    void ouvrirDialogue();

private:
    QPushButton *m_boutonDialogue2;

};


class MaFenetre5 : public QWidget
{
    Q_OBJECT

public:
    MaFenetre5();
public slots:
    void ouvrirDialogue();
    void ouvrirDossier();
private:
    QPushButton *m_boutonDialogue2;
    QPushButton *m_boutonDialogue3;
};











#endif // MAFENETRE_H
