#ifndef PERSONAGE_H_INCLUDED
#define PERSONAGE_H_INCLUDED

#include <string>
#include "Arme.h"

/*
On d�finit la classe avec ses m�thodes et attributs dans le .h
*/

class Personnage
{
//M�thode (Toujours en Public)
public:
    Personnage(std::string nom = "Inconnu");
    Personnage(std::string nomArme, int degatArme, std::string nom); //Surcharge du constructeur avec deux attributs d'initialisation
    Personnage(Personnage const& autre); //Modification du constructeur de copie (copie tous sua f le mana qui vaut 0
    ~Personnage(); //Destructeur de la classe Personnage qui permet de lib�rer la m�moire
    Personnage& operator=(Personnage const& personnageACopier);
    void recevoirDegat (int nbDegat) ;
    void attaquer (Personnage &cible) ; //S'applique sur la class Personage
    void bouleDeFeu (Personnage &cible) ;
    void boirePotionDeVie(int quantitePotion) ;
    void boirePotionDeMana(int quantitePotion) ;
    void changerArme (std::string nomNouvelleArme, int degatNouvelleArme) ;
    bool estVivant () const ;
    void afficherEtat() const ;
    std::string getNom () const;

//Attribut (Toujours en priv�)
private:
    int m_vie;
    int m_mana;
    Arme *m_arme ; //tilise la class arme
    std::string m_nom ;

};


#endif // PERSONAGE_H_INCLUDED
