#include <iostream>
#include <string>

#include "Personage.h"  //Contient la classe personage

using namespace std ;



/*
On définit les méthodes de la classe Personage dans le .cpp
La classe est déclaré dans le .h (header)
*/
    void Personnage::recevoirDegat (int nbDegat)
    {
        cout << getNom() << " recois " << nbDegat << " de dégat" << endl;
        m_vie -= nbDegat ;
        if (m_vie<0) {m_vie=0;}
    }

    void Personnage::attaquer (Personnage &cible) //S'applique sur la class Personage
    {
        cout << "Vous attaquez " << cible.getNom() << " pour infliger " << m_arme->getDegat() << " de dégat" << endl;
        cible.recevoirDegat(m_arme->getDegat()) ;
    }

    void Personnage::bouleDeFeu (Personnage &cible) //S'applique sur la class Personage
    {
        if (m_mana >= 30 )
        {
            cout << "Vous attaquez " << cible.getNom() << " pour infliger 30 de dégat" << endl;
            cible.recevoirDegat(30) ;
            m_mana -= 30 ;
        }
        else
        {
            m_mana = 0 ;
        }
        cout << "Mana restant : " << m_mana << endl ;
    }

    void Personnage::boirePotionDeVie(int quantitePotion)
    {
        cout << "Vous buvez une potion vous redonnant " << quantitePotion << " point de vie" << endl ;
        m_vie += quantitePotion ;
        if (m_vie>100){m_vie=100;}
        cout << "Vous avez " << m_vie << " point de vie" << endl ;
    }

    void Personnage::boirePotionDeMana(int quantitePotion)
    {
        cout << "Vous buvez une potion vous redonnant " << quantitePotion << " point de mana" << endl ;
        m_mana += quantitePotion ;
        if (m_mana>100){m_mana=100;}
        cout << "Vous avez " << m_mana << " point de mana" << endl ;
    }

    void Personnage::changerArme (string nomNouvelleArme, int degatNouvelleArme)
    {
        cout << "Vous vous équipez de " << nomNouvelleArme << " faisant " << degatNouvelleArme <<  " point de degat" << endl;
        m_arme->changer(nomNouvelleArme, degatNouvelleArme) ;
    }

    bool Personnage::estVivant() const // on définit une méthode en constante quand elle ne modifie pas les attributs
    {
        return m_vie>0 ;
    }

    void Personnage::afficherEtat() const
    {
        cout << "***********************" << endl ;
        cout << m_nom << endl;
        cout << "Vie = " << m_vie << endl ;
        cout << "Mana = " << m_mana << endl ;
        m_arme->afficherEtat() ;
        cout << "***********************" << endl ;
    }
    string Personnage::getNom() const
    {
        return m_nom ;
    }

/*
*************************************************************************************
Définission du constructeur afin d'initialisé les attributs de la classe Personage
*************************************************************************************
*/


Personnage::Personnage(string nom) :  m_arme(0)
{
    m_vie = 100 ;
    m_mana = 100 ;
    m_nom = nom ;
    m_arme = new Arme();
}

Personnage::Personnage(string nomArme, int degatArme, string nom) : m_vie(100), m_mana(100), m_nom(nom), m_arme(0)
{
    m_arme = new Arme(nomArme, degatArme) ;
}

Personnage::Personnage(Personnage const& autre) : m_vie(autre.m_vie), m_mana(0),
                                                  m_arme(0), m_nom(autre.m_nom)
{
    m_arme = (new Arme(*autre.m_arme));
}

Personnage::~Personnage()
{
    delete m_arme ;
}

//OPERATEUR DE COPIE (sert a copier les attribut d'un autre objet sur un objet du même type déja crée)
Personnage& Personnage::operator=(Personnage const& personnageACopier)
{
    if(this != &personnageACopier)
    //On vérifie que l'objet n'est pas le même que celui reçu en argument
    {
        m_vie = personnageACopier.m_vie; //On copie tous les champs
        m_mana = personnageACopier.m_mana;
	delete m_arme;
        m_arme = new Arme(*(personnageACopier.m_arme));
    }
    return *this; //On renvoie l'objet lui-même
}
