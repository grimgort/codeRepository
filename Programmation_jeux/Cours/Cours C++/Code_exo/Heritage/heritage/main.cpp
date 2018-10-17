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
La méthode coupdepoint demande en paramétre un personnage.
Cependant ici, mon guerrier est une classe fille de personnage et donc grâce à l'héritage,
on peut appliquer la méthode sur la fille soit guerrier.
*/
    monPersonnage.coupDePoing(monGuerrier);
    monGuerrier.coupDePoing(monPersonnage);



Personnage *monPersonnage2(0);
Guerrier *monGuerrier2 = new Guerrier();
/*
On peut affecter un objet mére à un objet fille Mais pas l'inverse!
En fait, on déplace le pointeur de mon personnage vers les attribut de mon guerrier commune aux deux class.
Si on fesait monguerrier=monPersonage, on déplacerai le pointeur de mon guerrier vers celui du personnage.
Hors, la class personnage posséde moins d'atribut que la classe guerrier qui est l'élément fille.
Par conséquent, les méthodes propre au guerrier ne renverrai sur aucun objet crer donc ca plante.
*/
monPersonnage2 = monGuerrier2; // Mais… mais… Ça marche !?


  Personnage marcel("Marcel");
   marcel.sePresenter();

   Guerrier lancelot("Lancelot du Lac");
   lancelot.sePresenter();

    return 0;
}
