#include "garage.h"
#include <iostream>

using namespace std;

Vehicule::Vehicule(int prix, int annee):m_prix(prix),m_annee(annee)
{}

void Vehicule::affiche() const
//J'en profite pour modifier un peu les fonctions d'affichage
{
    cout << "Ceci est un vehicule coutant " << m_prix << " euros de l'annee " << m_annee << endl;
}

Vehicule::~Vehicule() //Même si le destructeur ne fait rien, on doit le mettre !
{}

Voiture::Voiture(int prix, int portes, int annee):Vehicule(prix,annee), m_portes(portes)
{}

void Voiture::affiche() const
{
    cout << "Ceci est une voiture avec " << m_portes << " portes et coutant " << m_prix << " euros de l'annee " << m_annee << endl;
}

Voiture::~Voiture()
{}

Moto::Moto(int prix, double vitesseMax, int annee):Vehicule(prix,annee), m_vitesse(vitesseMax)
{}

void Moto::affiche() const
{
    cout << "Ceci est une moto allant a " << m_vitesse << " km/h et coutant " << m_prix << " euros de l'annee " << m_annee << endl;
}

Moto::~Moto()
{}

Camion::Camion(int prix, double poid, int annee):Vehicule(prix,annee), m_poid(poid)
{}

void Camion::affiche() const
{
    cout << "Ceci est un camion qui peut porter " << m_poid << " kg et coutant " << m_prix << " euros de l'annee " << m_annee << endl;
}

Camion::~Camion()
{}






void Garage::ajouterVoiture(int prix, int portes, int annee )
{
    m_listVehicule.push_back(new Voiture(prix,portes,annee));
}


void Garage::ajouterMoto(int prix, double vitesseMax, int annee )
{
    m_listVehicule.push_back(new Moto(prix,vitesseMax,annee));
}


void Garage::ajouterCamion(int prix, double poid, int annee )
{
    m_listVehicule.push_back(new Camion(prix,poid,annee));
}


void Garage::enlever(int numVehicule )
{
    delete m_listVehicule[numVehicule];
    m_listVehicule[numVehicule]=0;
}


void Garage::afficherVehicule()
{
    for (int i(0);i<m_listVehicule.size();i++)
    {
        m_listVehicule[i] -> affiche();
    }
}











