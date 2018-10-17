#ifndef GUERRIER_H_INCLUDED
#define GUERRIER_H_INCLUDED

#include <iostream>
#include <string>
#include "Personnage.h"
//Ne pas oublier d'inclure Personnage.h pour pouvoir en h�riter !

class Guerrier : public Personnage
{
    public:
        Guerrier(std::string nom = "Inconnue");
        void frapperCommeUnSourdAvecUnMarteau() const;
        //M�thode qui ne concerne que les guerriers
        void sePresenter() const;
};






#endif // GUERRIER_H_INCLUDED
