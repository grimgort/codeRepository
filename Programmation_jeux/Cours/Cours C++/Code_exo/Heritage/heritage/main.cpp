#include <iostream>
#include "personnage.h"
#include "guerrier.h"
#include "magicien.h"

using namespace std;

int main()
{
    Personnage monPersonnage;
    Guerrier monGuerrier;
/*
La m�thode coupdepoint demande en param�tre un personnage.
Cependant ici, mon guerrier est une classe fille de personnage et donc gr�ce � l'h�ritage,
on peut appliquer la m�thode sur la fille soit guerrier.
*/
    monPersonnage.coupDePoing(monGuerrier);
    monGuerrier.coupDePoing(monPersonnage);



Personnage *monPersonnage2(0);
Guerrier *monGuerrier2 = new Guerrier();
/*
On peut affecter un objet m�re � un objet fille Mais pas l'inverse!
En fait, on d�place le pointeur de mon personnage vers les attribut de mon guerrier commune aux deux class.
Si on fesait monguerrier=monPersonage, on d�placerai le pointeur de mon guerrier vers celui du personnage.
Hors, la class personnage poss�de moins d'atribut que la classe guerrier qui est l'�l�ment fille.
Par cons�quent, les m�thodes propre au guerrier ne renverrai sur aucun objet crer donc ca plante.
*/
monPersonnage2 = monGuerrier2; // Mais� mais� �a marche !?


  Personnage marcel("Marcel");
   marcel.sePresenter();

   Guerrier lancelot("Lancelot du Lac");
   lancelot.sePresenter();

    return 0;
}
