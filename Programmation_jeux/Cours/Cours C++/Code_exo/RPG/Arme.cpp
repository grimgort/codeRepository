
#include <string>
#include <iostream>

#include "Arme.h"

using namespace std ;

void Arme::changer(string nom, int degat)
{
    m_nom=nom ;
    m_degat=degat ;
}

void Arme::afficherEtat() const
{
    cout << "nom arme = " << m_nom << " / degat arme = " << m_degat << endl ;
}

int Arme::getDegat()const
{
    return m_degat ;
}




Arme::Arme() : m_nom("�p�e rouill�"), m_degat(10)
{

}

Arme::Arme(string nom, int degat) : m_nom(nom), m_degat(degat)
{

}
