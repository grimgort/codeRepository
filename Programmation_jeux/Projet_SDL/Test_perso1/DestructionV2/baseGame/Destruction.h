#ifndef DESTRUCTION_H_INCLUDED
#define DESTRUCTION_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

#include "Surface.h"

class Destruction : public Surface
{
public:
    Destruction();
//    ~Destruction();
    void mouseDestruction(int xm,int ym, int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Surface* surface, SDL_Event event);
    void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);


private:
    int m_rayon ;
};

#endif // DESTRUCTION_H_INCLUDED
