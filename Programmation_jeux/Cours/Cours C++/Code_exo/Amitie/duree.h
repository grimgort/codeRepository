#ifndef DUREE_H_INCLUDED
#define DUREE_H_INCLUDED

#include <iostream>


class Duree
{
    public:
    Duree(int heure = 0,int minute =0 ,int seconde=0);

    Duree& operator+=(const Duree& a) ; // L'op�rateur se situe dans les m�thode pour pouvoir modifier les attributs de la classe

    Duree& operator-=(const Duree& a) ;




    private:
        int m_heure ;
        int m_minute ;
        int m_seconde ;

    void afficher(std::ostream &flux) const; // dans la partie priv� pour emp�cher d'utiliser cette m�thode
    friend std::ostream &operator<<( std::ostream &flux, Duree const& duree); // friend permet d'utiliser la m�thode afficher.C'est le seul cr il est amis
//    bool estEgal(Duree const& b) const ; (inutile car l'op�rateur == peut maintenant acc�er aux attributs de la classe
    friend bool operator==(Duree const& a, Duree const& b);
//    bool estInferieur(Duree const& b) const ;
    friend bool operator<(Duree const&a, Duree const& b) ;
};


bool operator!=(Duree const& a, Duree const& b);
bool operator>(Duree const& a, Duree const& b) ;
bool operator<=(Duree const& a, Duree const& b) ;
bool operator>=(Duree const& a, Duree const& b) ;
Duree operator+(Duree const& a, Duree const& b) ;
Duree operator-(Duree const& a, Duree const& b) ;

std::ostream &operator<<( std::ostream &flux, Duree const& duree);

#endif // DUREE_H_INCLUDED
