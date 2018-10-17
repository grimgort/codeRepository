#ifndef SETTINGSSTATE_H
#define SETTINGSSTATE_H

#include <State.h>
#include <Player.h>
#include <Container.h>
#include <Button.h>
#include <Label.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>

class SettingsState : public State
{
    public:
        SettingsState(StateStack& stack, Context context);

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event& event);

    private:
        void updateLabels();
        void addButtonLabel(Player::Action action, float y, const std::string& text, Context context);

    private:
        sf::Sprite m_backgroundSprite;
        GUI::Container m_GUIContainer;
        std::array<GUI::Button::Ptr, Player::ActionCount> m_bindingButtons;
        std::array<GUI::Label::Ptr, Player::ActionCount> m_bindingLabels;

};

#endif // SETTINGSSTATE_H
