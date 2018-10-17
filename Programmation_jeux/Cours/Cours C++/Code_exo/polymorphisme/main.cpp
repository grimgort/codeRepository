#include <iostream>

#include <vector>
#include "garage.h"
using namespace std;

void presenter(Vehicule const& v)  //Présente le véhicule passé en argument
{
    v.affiche();
}

int main()
{
    Vehicule *v(0) ;
    v = new Vehicule(1000,1989) ;
    Vehicule *m(0) ;
    m = new Moto(1000,50,1989) ;

    m -> affiche() ;
    v -> affiche() ;

    delete m ;
    delete v ;

    vector<Vehicule*> listVehicule ;
    listVehicule.push_back(new Voiture(1000,4,1989)) ;
    listVehicule.push_back(new Moto(1000,4,2000)) ;
    listVehicule.push_back(new Camion(100000,10000,2500)) ;


    for (int i(0); i<listVehicule.size(); i++)
    {
        listVehicule[i] -> affiche();
        delete listVehicule[i];
        listVehicule[i]=0;
    }

//    Garage *GarageAMoi(0);
//    GarageAMoi -> new Garage();

    Garage GarageAMoi();
    GarageAMoi.ajouterCamion(1000,50,1989);
    GarageAMoi.ajouterVoiture(5000,4,1990);
    GarageAMoi.ajouterMoto(2000,250,1995);

    GarageAMoi.afficherVehicule();
    GarageAMoi.enlever(2);
    GarageAMoi.afficherVehicule();

    return 0;
}
