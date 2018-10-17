#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include <iostream>
#include <string>

class Personnage
{
    public:
        Personnage(std::string nom = "Inconnu") ;
        void recevoirDegats(int degats);
        void coupDePoing(Personnage &cible) const;
        void sePresenter() const ;

    protected: //Priv�, mais accessible aux �l�ments enfants (Guerrier, Magicien)
        int m_vie;
        std::string m_nom;
};


#endif // PERSONNAGE_H_INCLUDED
