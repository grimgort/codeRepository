#include "Destruction.h"
Destruction::Destruction() : m_rayon(40)
{

}


void Destruction::mouseDestruction(int xm,int ym, int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Surface* surface, SDL_Event event)
{
    if (xm<SCREEN_WIDTH-1 && xm > 0 && ym<SCREEN_HEIGHT-1 && ym >0) //Vérifie que la souris est compris dans l'écran (évite les plantage)
    {
        Uint32 couleurSouris = getPixelMap(surface,xm,ym);
    }

    //Si une touche a ete pressee
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        for (int i=xm-m_rayon; i<xm+m_rayon; i++)
        {
            if (i<SCREEN_WIDTH-1 && i>0)
            {

                for (int j=ym-m_rayon; j<ym+m_rayon; j++)
                {
                    if (j<SCREEN_HEIGHT-1 && j>0)
                    {
                        definirPixel(surface,i,j,SDL_MapRGB( surface->format, m_red, m_green, m_blue ));
                    }
                }
            }
        }
    }


}




void Destruction::definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /*Ici p est l'adresse du pixel que l'on veut modifier*/
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

