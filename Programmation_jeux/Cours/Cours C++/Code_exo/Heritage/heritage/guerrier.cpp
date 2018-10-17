#include <iostream>
#include <string>
#include "guerrier.h"

using namespace std ;


Guerrier::Guerrier(string nom) : Personnage(nom)
{

}


void Guerrier::sePresenter() const // C'est un masque. Cette fonction remplace celle de la fonction Personnage du même nom
{
    Personnage::sePresenter() ; // appel la fonction sePresenter de la classe personnage. C'est un Démasquage.
    cout << "Je suis un guerrier redoutable" << endl; // Rajoute cette ligne à la suite de la fonction sePresenté de perssonage.
}
