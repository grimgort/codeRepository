#ifndef SURFACE_H_INCLUDED
#define SURFACE_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

class Surface
{
public:
    Surface();
    Surface(int red,int green,int blue);
    ~Surface();
    void applySurface( int x, int y, SDL_Surface* destination );
    void loadImage( std::string filename );
    Uint32 getPixel(int x, int y);
    Uint32 getPixelMap(SDL_Surface* surface, int x, int y);
    SDL_Surface* getSurface() const;
    void mouseDestruction(int xm,int ym, int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Event event);
    void definirPixel(int x, int y, Uint32 pixel);


private:

protected:
    SDL_Surface* m_surface ;//Surface
    int m_x; //x position
    int m_y; //y position
    int m_red; //red color for mapRGB
    int m_green; //green color for mapRGB
    int m_blue; //blue color for mapRGB
    int m_rayon; //Rayon de destruction

};

#endif // SURFACE_H_INCLUDED
