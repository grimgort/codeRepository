#include "Personnage.h"
#include <math.h>


/*************************************
CONSTRUCTEUR
**************************************/
Personnage::Personnage() : Surface(),m_height(50),m_width(50),m_Vx(0.0),m_Vy(0.0),m_mouveX(0),m_mouveY(0),
                           m_mouveFy(0),m_mouveFx(0),m_masse(0.0)
{

}

Personnage::Personnage(int red,int green,int blue) :
    Surface(red,green,blue),m_height(50),m_width(50),m_Vx(0.0),m_Vy(0.0),m_mouveX(0),m_mouveY(0),
    m_mouveFy(0),m_mouveFx(0),m_masse(0.0)
{

}

Personnage::Personnage(int red,int green,int blue,int height,int width,double masse) :
    Surface(red,green,blue),m_height(height),m_width(width),m_Vx(0.0),m_Vy(0.0),m_mouveX(0),m_mouveY(0),
    m_mouveFy(0),m_mouveFx(0),m_masse(masse)
{

}

/*************************************
DESTRUCTEUR
**************************************/
//Personnage::~Personnage()
//{
//
//
//}

/*************************************
METHODE
**************************************/

void Personnage::collisionIni()
{
    for (int i=1; i<m_width-1; i++)
    {
        for (int j=1; j<m_height-1; j++)
        {
            if ((getPixel(i,j)==SDL_MapRGB( m_surface->format, m_red, m_green, m_blue)))
            {
                if ((getPixel(i-1,j)==SDL_MapRGB( m_surface->format, m_red, m_green, m_blue))&&
                        (getPixel(i+1,j)==SDL_MapRGB( m_surface->format, m_red, m_green, m_blue))&&
                        (getPixel(i,j-1)==SDL_MapRGB( m_surface->format, m_red, m_green, m_blue))&&
                        (getPixel(i,j+1)==SDL_MapRGB( m_surface->format, m_red, m_green, m_blue))) {}
                else
                {
                    coordCollisionX.push_back(i+m_x);
                    coordCollisionY.push_back(j+m_y);
                }
            }
        }
    }
}

void Personnage::handle_input(SDL_Event event)
{
    //Si une touche a ete pressee
    if( event.type == SDL_KEYDOWN )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
        case SDLK_UP:
            m_mouveY = -m_height/2;
            break;
        case SDLK_DOWN:
            m_mouveY = m_height/2;
            break;
        case SDLK_LEFT:
            m_mouveX = -m_width/2;
            break;
        case SDLK_RIGHT:
            m_mouveX = m_width/2;
            break;
        default:
            break;
        }
    }
    //Si une touche a ete relachee
    else if( event.type == SDL_KEYUP )
    {
        //ajustement de la vitesse
        switch( event.key.keysym.sym )
        {
        case SDLK_UP:
            m_mouveY = -m_height / 2;
            break;
        case SDLK_DOWN:
            m_mouveY = m_height / 2;
            break;
        case SDLK_LEFT:
            m_mouveX = -m_width / 2;
            break;
        case SDLK_RIGHT:
            m_mouveX = m_width / 2;
            break;
        default:
            break;
        }
    }
}

void Personnage::show(SDL_Surface* destination)
{
    applySurface( m_x, m_y, destination );
}


void Personnage::mouvement(SDL_Surface* surface,int screenWidth,int screenHeight)
{
    m_x +=  m_mouveX + m_mouveFx;
    m_y +=  m_mouveY + m_mouveFy;
    for (int i=0; i<=coordCollisionX.size(); i++)
    {
        coordCollisionX[i] += (m_mouveX + m_mouveFx);
    }
    for (int i=0; i<=coordCollisionY.size(); i++)
    {
        coordCollisionY[i] += (m_mouveY + m_mouveFy);
    }
//  Si le personnage se rapproche trop des limites de l'ecran
    if( ( m_x < 0 ) || ( m_x > screenWidth ) )
    {
        //On revient
        m_x -= (m_mouveX + m_mouveFx);
        m_Vx=0.0;
        for (int i=0; i<=coordCollisionX.size(); i++)
        {
            coordCollisionX[i] -= (m_mouveX + m_mouveFx);
        }
    }
    if( ( m_y < 0 ) || ( m_y > screenHeight ) )
    {
        //On revient
        m_y -= (m_mouveY + m_mouveFy);
        m_Vy=0.0;
        for (int i=0; i<=coordCollisionY.size(); i++)
        {
            coordCollisionY[i] -= (m_mouveY + m_mouveFy);
        }
    }

//  Si le personnage se rapproche trop du terrein
    for (int i=0; i<coordCollisionY.size(); i++)
    {
        for (int j=0; j<coordCollisionX.size(); j++)
        {
            if (getPixelMap(surface,coordCollisionX[j],coordCollisionY[i])!=SDL_MapRGB( surface->format, 255, 0, 255 ))
            {
                i=coordCollisionY.size();   // sort de la boucle i
                j=coordCollisionX.size();   // sort de la boucle j
                //On revient
                m_y -= (m_mouveY + m_mouveFy);
                m_x -= (m_mouveX + m_mouveFx);
                m_Vx=0.0;
                m_Vy=0.0;
                for (int i=0; i<=coordCollisionY.size(); i++)
                {
                    coordCollisionY[i] -= (m_mouveY + m_mouveFy);
                }
                for (int j=0; j<=coordCollisionX.size(); j++)
                {
                    coordCollisionX[j] -= (m_mouveX + m_mouveFx);
                }
            }
        }
    }
}

Uint32 Personnage::getPixelMap(SDL_Surface* surface, int x, int y)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /* Ici p est l'adresse du pixel que l'on veut connaitre */
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    //Gestion différente suivant le nombre d'octets par pixel de l'image
    switch(nbOctetsParPixel)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        //Suivant l'architecture de la machine
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    //Ne devrait pas arriver, mais évite les erreurs
    default:
        return 0;
    }

}

/***********************************************************
GRAVITE
***********************************************************/
/*
X1 = numero de pixel en x à t+1
Fx0 = Force en x à t
Vx0 = vitesse en x à t
masse = masse de l'objet
Vx1 = vitesse en x à t+1
masse = masse de l'objet
*/
void Personnage::mouveFx(double Fx, double deltaT)
{
    double coef(10000.0); //artificial coefficient for increase movement with velocity
    m_Vx += Fx/m_masse*deltaT ; //2nd Newton law
    m_mouveFx = floor(deltaT * m_Vx * coef) ;
}

/*
Z1 = numero de pixel en z à t+1
Fz0 = Force en z à t
Vz1 = vitesse en z à t+1
Vz0 = vitesse en z à t
masse = masse de l'objet
*/
void Personnage::mouveFy(double Fy, double deltaT)
{
    double coef(10000.0); //artificial coefficient for increase movement with velocity
    m_Vy += Fy/m_masse*deltaT ;
    m_mouveFy = floor(deltaT * m_Vy * coef) ;
}


void Personnage::initMouve()
{
    m_mouveX = 0;
    m_mouveY = 0;
}

