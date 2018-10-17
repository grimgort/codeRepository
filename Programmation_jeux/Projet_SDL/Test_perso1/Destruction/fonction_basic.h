#ifndef FONCTION_BASIC_H_INCLUDED
#define FONCTION_BASIC_H_INCLUDED


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
SDL_Surface *load_image( std::string filename );
void clean_up(SDL_Surface* fond, SDL_Surface* message,TTF_Font *font);
int updateScreen(SDL_Surface* screen);
int showText(const char* texte,SDL_Surface* screen,SDL_Surface* message,TTF_Font *font);
SDL_Surface *load_image2( std::string filename );















#endif // FONCTION_BASIC_H_INCLUDED
