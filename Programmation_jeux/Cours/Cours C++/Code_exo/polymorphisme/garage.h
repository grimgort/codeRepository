#ifndef GARAGE_H_INCLUDED
#define GARAGE_H_INCLUDED

#include <vector>
#include <iostream>



class Vehicule
{
    public:
    Vehicule(int prix,int annee);           //Construit un véhicule d'un certain prix
    virtual void affiche() const;
    virtual ~Vehicule();          //Remarquez le 'virtual' ici

    protected:
    int m_prix;
    int m_annee;
};

class Voiture: public Vehicule
{
    public:
    Voiture(int prix, int portes,int annee);
    //Construit une voiture dont on fournit le prix et le nombre de portes
    virtual void affiche() const;
    virtual ~Voiture();

    private:
    int m_portes;
};

class Moto : public Vehicule
{
    public:
    Moto(int prix, double vitesseMax,int annee);
    //Construit une moto d'un prix donné et ayant une certaine vitesse maximale
    virtual void affiche() const;
    virtual ~Moto();

    private:
    double m_vitesse;
};

class Camion : public Vehicule
{
public:
    Camion(int prix, double poid,int annee);
    virtual void affiche() const ;
    virtual~Camion();


private:
    double m_poid ;
};

class Garage
{
public:
    Garage();
    virtual ~Garage();
    void ajouterVoiture(int prix,int portes,int annee);
    void ajouterMoto(int prix,double vitessemax,int annee);
    void ajouterCamion(int prix,double poid, int annee);

    void enlever(int numVehicule);

    void afficherVehicule();


private:
    std::vector<Vehicule*> m_listVehicule ;
};


#endif // GARAGE_H_INCLUDED
