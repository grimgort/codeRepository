#ifndef PRESENTATIONSTATE_H
#define PRESENTATIONSTATE_H

#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

/////////////////////////////////////////////////
/// \file PresentationState.h
/// \brief Permet l'affichage d'un �tat de pr�sentation des cr�ateurs
/// \author Wiwi
/// \version 0.0
///
/// Etat qui s'affiche avant le menu du jeu
/// Il permet de rendre hommage aux cr�ateurs et autres
/////////////////////////////////////////////////

class PresentationState : public State
{
    public:
        PresentationState(StateStack& stack, Context context);

        virtual void draw();
        virtual bool update (sf::Time dt);
        virtual bool handleEvent (const sf::Event& event);

    private:
        sf::Sprite m_backgroundSprite;
        sf::Clock m_spriteClock;
};

#endif // PRESENTATIONSTATE_H
