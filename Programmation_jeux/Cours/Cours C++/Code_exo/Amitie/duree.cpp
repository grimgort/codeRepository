#include "duree.h"
#include <iostream>
using namespace std ;

Duree::Duree(int heure, int minute, int seconde) : m_heure(heure), m_minute(minute),m_seconde(seconde)
{

}




bool operator==(Duree const& a, Duree const& b)
{
    return (a.m_heure == b.m_heure && a.m_minute == b.m_minute && a.m_seconde == b.m_seconde);
}
bool operator!=(Duree const& a, Duree const&b ) // l'opérateur != utilise la fonction == pour faire la comparaison. on a donc pas besoin de définir une méthode estDifferent.
{
    return !(a==b);
}
//bool Duree::estEgal(Duree const& b) const
//{
//    return (m_heure == b.m_heure && m_minute == b.m_minute && m_seconde == b.m_seconde);
//}

bool operator<(Duree const& a, Duree const& b)
{
    if (a.m_heure<b.m_heure)
        return true ;
    else if (a.m_heure == b.m_heure && a.m_minute < b.m_minute)
        return true ;
    else if (a.m_heure == b.m_heure && a.m_minute==b.m_minute & a.m_seconde < b.m_seconde)
        return true ;
    else
        return false ;
}


bool operator>(Duree const& a, Duree const& b)
{
    return !(a<b) ;
}

bool operator<=(Duree const& a, Duree const& b)
{
    return (a<b || a==b ) ;
}

bool operator>=(Duree const& a, Duree const& b)
{
    return !(a<b || a!=b ) ;
}

Duree& Duree::operator+=(const Duree& a)
{
    m_seconde += a.m_seconde ;
    m_minute += m_seconde / 60 ;
    m_seconde %= 60 ;

    m_minute += a.m_minute ;
    m_heure += m_minute/60 ;
    m_minute %= 60 ;

    m_heure += a.m_heure ;
    return *this ;
}


Duree operator+(Duree const& a, Duree const& b)
{
    Duree copie(a);
    copie += b ;
    return copie ;
}

Duree& Duree::operator-=(const Duree& a)
{
    int temps1(m_seconde+m_minute*60+m_heure*3600);
    int temps2(a.m_seconde+a.m_minute*60+a.m_heure*3600);
    int total (temps1-temps2);
    m_heure = total/3600 ;
    m_minute = (total % 3600) / 60;
    m_seconde = total-m_heure*3600-m_minute*60 ;
    return *this ;
}

Duree operator-(Duree const& a, Duree const& b)
{
    Duree copie(a);
    copie -= b ;
    return copie ;
}

void Duree::afficher(ostream &flux) const
{
    flux << m_heure << "h" << m_minute << "m" << m_seconde << "s";
}

ostream &operator<<( ostream &flux, Duree const& duree)
{
    duree.afficher(flux) ;
    return flux;
}



