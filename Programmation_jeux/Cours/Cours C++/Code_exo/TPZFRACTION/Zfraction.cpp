
#include <iostream>
#include "Zfraction.h"

using namespace std ;
/*
*************************************************
CONSTRUCTEUR
*************************************************
*/
Zfraction::Zfraction(int a, int b) : m_numerateur(a) , m_denominateur(b)
{
    int c(pgcd(m_numerateur,m_denominateur));
    m_numerateur /= c ;
    m_denominateur /= c ;
    signe()
}

/*
*************************************************
SURCHARGE
*************************************************
*/
//COUT
void Zfraction::afficher(ostream &flux) const
{
    if (m_denominateur == 1)
    {
        flux << m_numerateur ;
    }
    else
    {
        flux << m_numerateur << "/" << m_denominateur ;
    }
}
ostream &operator<<(ostream& flux, Zfraction const& Zfraction)
{
    Zfraction.afficher(flux);
    return flux ;
}

//ADDITION
Zfraction& Zfraction::operator+=(const Zfraction& a)
{
    m_numerateur = m_numerateur * a.m_denominateur + a.m_numerateur * m_denominateur;
    m_denominateur *= a.m_denominateur;
    int b(pgcd(m_numerateur,m_denominateur));
    m_numerateur /= b ;
    m_denominateur /= b ;
    return *this ;

}
Zfraction operator+(Zfraction const& a, Zfraction const& b)
{
    Zfraction copie(a);
    copie += b ;
    return copie ;
}
//MULTIPLICATION
Zfraction& Zfraction::operator*=(const Zfraction& a)
{
    m_numerateur = m_numerateur * a.m_numerateur ;
    m_denominateur *= a.m_denominateur;
    int b(pgcd(m_numerateur,m_denominateur));
    m_numerateur /= b ;
    m_denominateur /= b ;
    return *this ;

}
Zfraction operator*(Zfraction const& a, Zfraction const& b)
{
    Zfraction copie(a);
    copie *= b ;
    return copie ;
}
//DIVISION
Zfraction& Zfraction::operator/=(const Zfraction& a)
{
    m_numerateur = m_numerateur * a.m_denominateur ;
    m_denominateur *= a.m_numerateur;
    int b(pgcd(m_numerateur,m_denominateur));
    m_numerateur /= b ;
    m_denominateur /= b ;
    return *this ;

}
Zfraction operator/(Zfraction const& a, Zfraction const& b)
{
    Zfraction copie(a);
    copie /= b ;
    return copie ;
}
//SOUSTRACTION
Zfraction& Zfraction::operator-=(const Zfraction& a)
{
    m_numerateur = m_numerateur * a.m_denominateur - a.m_numerateur * m_denominateur;
    m_denominateur *= a.m_denominateur;
    int b(pgcd(m_numerateur,m_denominateur));
    m_numerateur /= b ;
    m_denominateur /= b ;
    return *this ;

}
Zfraction operator-(Zfraction const& a, Zfraction const& b)
{
    Zfraction copie(a);
    copie -= b ;
    return copie ;
}
//OPPOSE
Zfraction operator-(Zfraction const& autre)
{
    Zfraction copie(autre);
    copie *= -1;
    return autre;
}

//INFERIEUR
bool Zfraction::estInferieur(Zfraction const& b) const
{
    int an(m_numerateur * b.m_denominateur) ;
    int bn(b.m_numerateur * m_numerateur) ;
    return (an<bn);
}
bool operator<(Zfraction const& a, Zfraction const& b)
{
    return a.estInferieur(b) ;
}
// SUPERIEUR
bool operator>(Zfraction const& a, Zfraction const& b)
{
    return b.estInferieur(a) ;;
}
//EGAL
bool Zfraction::estEgal(Zfraction const& b) const
{
    int an(m_numerateur * b.m_denominateur) ;
    int bn(b.m_numerateur * m_numerateur) ;
    return (an==bn);
}
bool operator==(Zfraction const& a, Zfraction const& b)
{
    return a.estEgal(b) ;
}
//DIFFERENT
bool operator!=(Zfraction const& a, Zfraction const& b)
{
    return !(a.estEgal(b)) ;
}
//INFERIEUR OU EGAL
bool operator<=(Zfraction const& a, Zfraction const& b)
{
    return  ((a==b) || (a<b));
}
//SUPERIEUR OU EGAL
bool operator>=(Zfraction const& a, Zfraction const& b)
{
    return  ((a==b) || (a>b));
}
//GESTION SIGNE
void Zfraction::signe()
{
    //Si le signe est au dénominateur seulement
    if(m_numerateur >= 0 && m_denominateur <0)
    {
        m_numerateur = -m_numerateur;
        m_denominateur = -m_denominateur;
    }

    //Si le signe est au numérateur et au dénominateur
    else if(m_numerateur < 0 && m_denominateur <0)
    {
        m_numerateur = -m_numerateur;
        m_denominateur = -m_denominateur;
    }

    //Dans tous les autres cas, il n'y a rien à faire
}



/*
*************************************************
FONCTION
*************************************************
*/
//PGCD
int pgcd(int a, int b)
{
    while (b != 0)
    {
        const int t = b;
        b = a%b;
        a=t;
    }
    return a;
}
//RENVOI LE NUMERATEUR
void Zfraction::numerateur()
{
    cout << m_numerateur << endl ;
}
//RENVOI LE DENOMINATEUR
void Zfraction::denominateur()
{
    cout << m_denominateur << endl ;
}
//RENVOI LE RESULTAT DE LA FRACTION EN REEL
double Zfraction::reel()
{
    return (static_cast<double>(m_numerateur)/static_cast<double>(m_denominateur)) ;
}




