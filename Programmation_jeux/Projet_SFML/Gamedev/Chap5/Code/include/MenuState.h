#ifndef MENUSTATE_H
#define MENUSTATE_H
#include <State.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State
{
    public:
        MenuState(StateStack& stack, Context context);

        virtual void draw();
        virtual bool update(sf::Time  dt);
        virtual bool handleEvent(const sf::Event& event);

        void updateOptionText();

    private :
        enum OptionNames
        {
            Play,
            Exit,
        };

    private:
        sf::Sprite m_backgroundSprite;

        std::vector<sf::Text> m_options;
        std::size_t m_optionIndex;


};

#endif // MENUSTATE_H
