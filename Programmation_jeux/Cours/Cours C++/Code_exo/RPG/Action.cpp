#include <iostream>
#include <string>
#include <vector>
#include "Action.h"  //Contient la classe personage
#include "Personage.h"


using namespace std ;

void attaquer()
{


/*
    int fin=3;
    Joueur[] j = new Joueur[fin-1];

    for(int i=1; i<=fin; i++){
    Joueur j[i-1] = new Joueur("Jones");// indice d'un tableau commence à 0

}
*/
}





void magie()
{

}

void equipement()
{

}

void choixAction(Personnage *listePersonnage[])
{
    int actionChoisi ;
    cout << "1=attaquer / 2=lancer un sort / 3=equipement / "  << endl ;
    cin >> actionChoisi ;
    switch (actionChoisi)
    {
    case 1:
        attaquer() ;
        break;
    case 2:
        magie() ;
        break;
    case 3:
        equipement() ;
        break;

    }


}




