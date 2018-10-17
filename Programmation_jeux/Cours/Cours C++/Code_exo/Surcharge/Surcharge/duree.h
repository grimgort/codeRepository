#ifndef DUREE_H_INCLUDED
#define DUREE_H_INCLUDED

#include <iostream>


class Duree
{
    public:
    Duree(int heure = 0,int minute =0 ,int seconde=0);
    bool estEgal(Duree const& b) const ;
    bool estInferieur(Duree const& b) const ;
    Duree& operator+=(const Duree& a) ; // L'opérateur se situe dans les méthode pour pouvoir modifier les attributs de la classe
    void afficher(std::ostream &flux) const;
    Duree& operator-=(const Duree& a) ;




    private:
        int m_heure ;
        int m_minute ;
        int m_seconde ;


};

bool operator==(Duree const& a, Duree const& b);
bool operator!=(Duree const& a, Duree const& b);
bool operator<(Duree const&a, Duree const& b) ;
bool operator>(Duree const& a, Duree const& b) ;
bool operator<=(Duree const& a, Duree const& b) ;
bool operator>=(Duree const& a, Duree const& b) ;
Duree operator+(Duree const& a, Duree const& b) ;
Duree operator-(Duree const& a, Duree const& b) ;

std::ostream &operator<<( std::ostream &flux, Duree const& duree);

#endif // DUREE_H_INCLUDED
