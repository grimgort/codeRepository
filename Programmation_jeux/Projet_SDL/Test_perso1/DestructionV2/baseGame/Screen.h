#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>


class Screen
{
    //Méthode
public:
    Screen(); //constructeur
    virtual ~Screen(); //Destructeur
    bool init() ;
    void updateScreen();
    void mousePosition()  ;
    SDL_Surface* getScreen() const;
    int getHeight() const;
    int getWidth() const;
    int getMouseX() const;
    int getMouseY() const;



    //Attribut
private:
    SDL_Surface* m_screen ;//La fenêtre
    const int m_screen_WIDTH ; // la largeur de la fenêtre
    const int m_screen_HEIGHT ; // la hauteur de la fenêtre
    const int m_screen_BPP ; // le nombre de bit de la fenêtre
    const int m_FRAMES_PER_SECOND ; // le nombre de fram par seconde
    const char* m_caption ; // Le titre de la fenêtre
    int m_mouseX;
    int m_mouseY;

};

#endif // SCREEN_H_INCLUDED
