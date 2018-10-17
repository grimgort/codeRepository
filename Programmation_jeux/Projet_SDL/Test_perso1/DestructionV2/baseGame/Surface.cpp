

#include "Surface.h"
/*************************************
CONSTRUCTEUR
**************************************/
Surface::Surface() : m_surface(NULL),m_x(0),m_y(0),m_red(-9999),m_green(-9999),m_blue(-9999),m_rayon(40)
{

}

Surface::Surface(int red,int green,int blue) : m_surface(NULL),m_x(0),m_y(0), m_red(red),m_green(green),m_blue(blue),
                                               m_rayon(40)
{

}

/*************************************
DESTRUCTEUR
**************************************/
Surface::~Surface()
{
//    SDL_FreeSurface(m_surface);
    delete m_surface;

}

/*************************************
METHODE
**************************************/
/*
Description : apply surface to screen
*/
void Surface::applySurface( int x, int y, SDL_Surface* destination )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( m_surface, NULL, destination, &offset );
    m_x = x ;
    m_y = y ;
}
/*
Description : load a picture to surface
*/
void Surface::loadImage( std::string filename )
{
    //L'image qui est chargée
    SDL_Surface* loadedImage = NULL;

    //L'image optimisée que nous utiliserons par la suite
    SDL_Surface* optimizedImage = NULL;

    //Chargement de l'image
    loadedImage = IMG_Load( filename.c_str() );

    //Si l'image est chargée
    if( loadedImage != NULL )
    {
        //creation de l'image optimisée
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //liberation de l'ancienne image
        SDL_FreeSurface( loadedImage );

        //si l'image optimisée créé est bonne
        if( optimizedImage != NULL )
        {
            if (m_red!=-9999 && m_green !=-9999 && m_blue !=-9999)
            {
             Uint32 colorkey = SDL_MapRGB( optimizedImage->format, m_red, m_green, m_blue );
             SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
            }
        }
    }

    //on retourne l'image optimisé
    m_surface = optimizedImage;
}

/*
obtenirPixel : permet de récupérer la couleur d'un pixel
Paramètres d'entrée/sortie :
SDL_Surface *surface : la surface sur laquelle on va récupérer la couleur d'un pixel
int x : la coordonnée en x du pixel à récupérer
int y : la coordonnée en y du pixel à récupérer
Uint32 resultat : la fonction renvoie le pixel aux coordonnées (x,y) dans la surface
*/
Uint32 Surface::getPixel(int x, int y)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = m_surface->format->BytesPerPixel;
    /* Ici p est l'adresse du pixel que l'on veut connaitre */
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)m_surface->pixels + y * m_surface->pitch + x * nbOctetsParPixel;

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

Uint32 Surface::getPixelMap(SDL_Surface* surface, int x, int y)
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


void Surface::mouseDestruction(int xm,int ym, int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Event event)
{
    if (xm<SCREEN_WIDTH-1 && xm > 0 && ym<SCREEN_HEIGHT-1 && ym >0) //Vérifie que la souris est compris dans l'écran (évite les plantage)
    {
        Uint32 couleurSouris = getPixel(xm,ym);
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
                        definirPixel(i,j,SDL_MapRGB( m_surface->format, m_red, m_green, m_blue ));
                    }
                }
            }
        }
    }


}

void Surface::definirPixel(int x, int y, Uint32 pixel)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = m_surface->format->BytesPerPixel;
    /*Ici p est l'adresse du pixel que l'on veut modifier*/
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)m_surface->pixels + y * m_surface->pitch + x * nbOctetsParPixel;

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




/*************************************
ACCESSEUR
**************************************/
SDL_Surface* Surface::getSurface() const
{
    return m_surface;
}
