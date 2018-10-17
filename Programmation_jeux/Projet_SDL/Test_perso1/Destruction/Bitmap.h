#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED
Uint32 get_pixel32( int x, int y, SDL_Surface *surface );
Uint32 obtenirPixel(SDL_Surface *surface, int x, int y);
void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

#endif // BITMAP_H_INCLUDED
