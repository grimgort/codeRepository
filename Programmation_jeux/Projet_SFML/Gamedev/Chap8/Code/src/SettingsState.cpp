#include <SettingsState.h>
#include <Utility.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack& stack,Context context):
    State(stack, context),
    m_GUIContainer()
{
    m_backgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    //construction des boutton et leur label
    addButtonLabel(Player::MoveLeft, 150.f, "Move Left", context);
    addButtonLabel(Player::MoveRight, 200.f, "Move Right", context);
    addButtonLabel(Player::MoveUp, 250.f, "Move Up", context);
    addButtonLabel(Player::MoveDown, 300.f, "Move Down", context);
    addButtonLabel(Player::Fire, 350.f, "Fire", context);
	addButtonLabel(Player::LaunchMissile, 400.f, "Missile", context);

    updateLabels();

    auto backButton = std::make_shared<GUI::Button> (*context.fonts, *context.textures);
    backButton->setPosition(80.f, 450.f);
    backButton->setText("Back");
    backButton->setCallBack(std::bind(&SettingsState::requestStackPop, this));

    m_GUIContainer.pack(backButton);
}

void SettingsState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    window.draw(m_backgroundSprite);
    window.draw(m_GUIContainer);
}

bool SettingsState::update(sf::Time)
{
    return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
    bool isKeyBinding = false;

    //on attend que le joueur appuie sur un button, en itérant sur les button et vérifiant qu'ils ont un statue actif
    for (std::size_t action = 0; action < Player::ActionCount; ++ action )
    {
        if (m_bindingButtons[action]->isActive())
        {
            isKeyBinding = true;
            if (event.type == sf::Event::KeyReleased)
            {
                getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
                m_bindingButtons[action]->deactivate();
            }
            break;
        }
    }

    //si le boutton pressé change de Label on met a jour le label, sinon on regarde les autres button
    if (isKeyBinding)
		updateLabels();
    else
		m_GUIContainer.handleEvent(event);

    return false;
}

void SettingsState::updateLabels()
{
    Player& player = *getContext().player;

    for (std::size_t i = 0; i < Player::ActionCount; i++)
    {
        sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
        m_bindingLabels[i]->setText(toString(key));
    }
}

void SettingsState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{
    m_bindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    m_bindingButtons[action]->setPosition(80.f, y);
    m_bindingButtons[action]->setText(text);
    m_bindingButtons[action]->setToggle(true);

    m_bindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
    m_bindingLabels[action]->setPosition(300.f, y + 15.f);

    m_GUIContainer.pack(m_bindingButtons[action]);
    m_GUIContainer.pack(m_bindingLabels[action]);
}





