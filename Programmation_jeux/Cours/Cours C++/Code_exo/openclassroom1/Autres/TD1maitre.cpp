#include <iostream>     // iostream est la bibliothéque qui permet de faire les entrée sortie de la console.
#include <string>   // Permet d'utiliser des variables de type string
#include <cmath>    // Permet d'inclure les fonction mathématique de base (sqrt, exp, etc...)
#include "TD1.h"


using namespace std ;   /* Permet de spécifier que l'on utilise la gestion
de la console grace au lot std définit dans la bibliothéque iostream.
Cela permet d'éviter les conflit quans plusieurs bibliothéque avec des fonctionnalité différentes sont chargés. */


int main()  // Délimite les bornes d'execution du programme
{
    multiJeux() ;

return 0 ;  /* Définit la fin de la fonction.
    Le "0" permet de renvoyer 0 si l'execution de la console c'est bien déroulé.
    Sinon le chiffre renvoyé serat différent de 0 */
}
