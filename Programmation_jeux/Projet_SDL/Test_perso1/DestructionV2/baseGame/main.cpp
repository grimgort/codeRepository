#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

#include "Screen.h"
#include "Surface.h"
#include "Personnage.h"
#include "Timer.h"
#include "Destruction.h"

int main ( int argc, char** argv )
{
    Timer Time ;
    Screen FenetrePrincipal;

    Surface Ciel ;
    Ciel.loadImage( "ciel.png" );
    Ciel.applySurface( 1, 1, FenetrePrincipal.getScreen());

    Surface Terrain(255,0,255) ;
    Terrain.loadImage("more-trees.png");
    Terrain.applySurface(1,1,FenetrePrincipal.getScreen());

    Personnage Tux(0,255,255,32,32,70.) ;
    Tux.loadImage("tux-sit.png");
    Tux.applySurface(100,100,FenetrePrincipal.getScreen());
    Tux.collisionIni();



    double Fx(0.0);
    double Fy(9.8); //gravity. Force is false. it s not gravity acceleration. To change it
    double deltaT(0.0);



    SDL_Event event;    //Variable d'événement
    bool quit = false;
    while( quit == false )
    {
        Time.start();
        Tux.initMouve();
        FenetrePrincipal.mousePosition();
        while( SDL_PollEvent( &event ) )
        {
            Tux.handle_input(event);

            Terrain.mouseDestruction(FenetrePrincipal.getMouseX(),FenetrePrincipal.getMouseY(),FenetrePrincipal.getWidth(),
                                         FenetrePrincipal.getHeight(),event);

            //Si l'utilisateur ferme la fenetre avec le X
            if( event.type == SDL_QUIT )
            {
                //On quitte le programme
                quit = true;
            }
        }
        FenetrePrincipal.updateScreen();    //met à jour la fenêtre principal pour prendre en compte les event
        Time.standFrame(60);


        deltaT = static_cast<double>(Time.getTicks())/1000. ; //temps en secondes
        Tux.mouveFx(Fx,deltaT);
        Tux.mouveFy(Fy,deltaT);

        Tux.mouvement(Terrain.getSurface(),FenetrePrincipal.getWidth(),FenetrePrincipal.getHeight());





        Ciel.applySurface( 1, 1, FenetrePrincipal.getScreen());
        Terrain.applySurface(1,1,FenetrePrincipal.getScreen());
        Tux.show(FenetrePrincipal.getScreen());





    }

// A appeler pour libérer la mémoire mais arret bizare avec...
    Terrain.~Surface();
//   Ciel.~Surface();
//   Tux.~Surface();
//   FenetrePrincipal.~Screen(); // Destructeur de la fenêtre principal
}
