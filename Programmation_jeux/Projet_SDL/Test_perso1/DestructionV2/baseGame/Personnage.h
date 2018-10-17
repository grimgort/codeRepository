#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

#include "Surface.h"

class Personnage : public Surface
{
public:
    Personnage();
    Personnage(int red,int green, int blue);
    Personnage(int red,int green,int blue,int height,int width,double masse);
//virtual ~Personnage();
    void handle_input(SDL_Event event);
    void collisionIni();
    void position();
    void show(SDL_Surface* destination);
    void mouvement(SDL_Surface* surface,int screenWidth,int screenHeight);
    Uint32 getPixelMap(SDL_Surface* surface, int x, int y);
    void mouveFx(double Fx, double deltaT);
    void mouveFy(double Fy, double deltaT);
    void initMouve();
private:
    int m_height;
    int m_width;
    int m_mouveY;
    int m_mouveX;
    std::vector<int> coordCollisionX;
    std::vector<int> coordCollisionY;
    double m_masse;
    double m_Vx;
    double m_Vy;
    int m_mouveFx ;
    int m_mouveFy ;
};

#endif // PERSONNAGE_H_INCLUDED
