#ifndef ZFRACTION_H_INCLUDED
#define ZFRACTION_H_INCLUDED

#include <iostream>

class Zfraction
{
public:
    //Constructeur
    Zfraction(int a=0, int b=1) ;
    //Surcharge d'operateur
    void afficher(std::ostream &flux) const ;
    Zfraction& operator+=(const Zfraction& a);
    Zfraction& operator*=(const Zfraction& a);
    bool estInferieur(Zfraction const& b) const;
    bool estEgal(Zfraction const& b) const ;
    Zfraction& operator/=(const Zfraction& a) ;
    Zfraction& operator-=(const Zfraction& a);
    //METHODE
    void numerateur() ;
    void denominateur() ;
    double reel() ;
    void signe();

private:
    int m_numerateur ;
    int m_denominateur ;

};

std::ostream &operator<<(std::ostream& flux, Zfraction const& Zfraction) ;
Zfraction operator+(Zfraction const& a, Zfraction const& b);
Zfraction operator*(Zfraction const& a, Zfraction const& b);
bool operator<(Zfraction const& a, Zfraction const& b) ;
bool operator>(Zfraction const& a, Zfraction const& b) ;
bool operator==(Zfraction const& a, Zfraction const& b) ;
bool operator!=(Zfraction const& a, Zfraction const& b) ;
bool operator<=(Zfraction const& a, Zfraction const& b) ;
bool operator>=(Zfraction const& a, Zfraction const& b) ;
Zfraction operator/(Zfraction const& a, Zfraction const& b) ;
Zfraction operator-(Zfraction const& a, Zfraction const& b) ;
Zfraction operator-(Zfraction const& a);

int pgcd(int a, int b) ;



#endif // ZFRACTION_H_INCLUDED
