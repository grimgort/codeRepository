#include <iostream>     // iostream est la biblioth�que qui permet de faire les entr�e sortie de la console.
#include <string>   // Permet d'utiliser des variables de type string
#include <cmath>    // Permet d'inclure les fonction math�matique de base (sqrt, exp, etc...)
#include "Fonction1.h"
#include "TD1.h"


using namespace std ;   /* Permet de sp�cifier que l'on utilise la gestion
de la console grace au lot std d�finit dans la biblioth�que iostream.
Cela permet d'�viter les conflit quans plusieurs biblioth�que avec des fonctionnalit� diff�rentes sont charg�s. */



int main()  // D�limite les bornes d'execution du programme
{

//    partie1() ;


    ecriture1() ;
    ecriture2() ;
    ecriture3() ;
    ecriture4() ;

    lecture1();

    positionFichier() ;

    tailleFichier() ;

    multiJeux() ;













    return 0 ;  /* D�finit la fin de la fonction.
    Le "0" permet de renvoyer 0 si l'execution de la console c'est bien d�roul�.
    Sinon le chiffre renvoy� serat diff�rent de 0 */
}
