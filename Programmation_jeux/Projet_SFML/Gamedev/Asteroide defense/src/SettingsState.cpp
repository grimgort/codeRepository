#include <SettingsState.h>
#include <Utility.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack& stack,Context context):
    State(stack, context),
    m_GUIContainer()
{
    m_backgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    for (std::size_t x = 0; x < 2; ++x)
    {
        //construction des boutton et leur label
        addButtonLabel(PlayerAction::MoveLeft, x, 0, "Move Left", context);
        addButtonLabel(PlayerAction::MoveRight, x, 1, "Move Right", context);
        addButtonLabel(PlayerAction::MoveUp, x, 2, "Move Up", context);
        addButtonLabel(PlayerAction::MoveDown, x, 3, "Move Down", context);
        addButtonLabel(PlayerAction::Fire, x, 4, "Fire", context);
        addButtonLabel(PlayerAction::LaunchMissile, x, 5, "Missile", context);
    }

    updateLabels();

    auto backButton = std::make_shared<GUI::Button> (context);
    backButton->setPosition(80.f, 620.f);
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
    for (std::size_t i = 0; i < 2*PlayerAction::Count; ++i)
    {
        if (m_bindingButtons[i]->isActive())
        {
            isKeyBinding = true;
            if (event.type == sf::Event::KeyReleased)
            {
                if (i < PlayerAction::Count)
                    getContext().keys1->assignKey(static_cast<PlayerAction::Type>(i), event.key.code);
                else
                    getContext().keys2->assignKey(static_cast<PlayerAction::Type>(i - PlayerAction::Count), event.key.code);
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
    for (std::size_t i = 0; i < PlayerAction::Count; ++i)
    {
        auto action = static_cast<PlayerAction::Type>(i);

        sf::Keyboard::Key key1 = getContext().keys1->getAssignedKey(action);
        sf::Keyboard::Key key2 = getContext().keys2->getAssignedKey(action);

        m_bindingLabels[i]->setText(toString(key1));
        m_bindingLabels[i + PlayerAction::Count]->setText(toString(key2));
    }
}

void SettingsState::addButtonLabel(std::size_t index, std::size_t x, std::size_t y, const std::string& text, Context context)
{
    index += PlayerAction::Count * x;

    m_bindingButtons[index] = std::make_shared<GUI::Button>(context);
    m_bindingButtons[index]->setPosition(400.f*x + 80.f, 50.f*y + 300.f);
    m_bindingButtons[index]->setText(text);
    m_bindingButtons[index]->setToggle(true);

    m_bindingLabels[index] = std::make_shared<GUI::Label>("", *context.fonts);
    m_bindingLabels[index]->setPosition(400.f*x + 300.f, 50.f*y + 315.f);

    m_GUIContainer.pack(m_bindingButtons[index]);
    m_GUIContainer.pack(m_bindingLabels[index]);
}
