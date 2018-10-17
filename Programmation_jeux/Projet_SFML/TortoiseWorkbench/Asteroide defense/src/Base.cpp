#include "Base.h"
#include <DataTables.h>
#include <Utility.h>

#include <CommandQueue.h>
#include <SoundNode.h>
#include <NetworkNode.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

using namespace std::placeholders;

namespace
{
const std::vector<BaseData> Table = initializeBaseData();
}


Base::Base (Type type
            , const TextureHolder& textures
            , const FontHolder& fonts)
    : Entity (Table[type].hitpoints)
    , m_type (type)
    , m_sprite (textures.get (Table[type].texture))
    , m_showExplosion (true)
    , m_explosionBegan (false)
    , m_explosion (textures.get (Textures::Explosion))
{
    m_explosion.setFrameSize (sf::Vector2i (256, 256));
    m_explosion.setNumFrames (16);
    m_explosion.setDuration (sf::seconds (1));

    //   centerOrigin(m_sprite);
    centerOrigin (m_explosion);

    //initialisation du text
    std::unique_ptr<TextNode> healthDisplay (new TextNode (fonts, ""));
    m_healthDisplay = healthDisplay.get();
    attachChild (std::move (healthDisplay));

    updateTexts();
}


unsigned int Base::getCategory() const
{
    return Category::Base;
}

sf::FloatRect Base::getBoundingRect() const
{
    return getWorldTransform().transformRect (m_sprite.getGlobalBounds());
}

bool Base::isMarkedForRemoval() const
{
    return isDestroyed()
           && (m_explosion.isFinished() || !m_showExplosion);
}

void Base::remove()
{
    Entity::remove();
    m_showExplosion = false;
}

void Base::drawCurrent (sf::RenderTarget& target
                        , sf::RenderStates states) const
{
    if (isDestroyed() && m_showExplosion)
    {
        target.draw (m_explosion, states);
    }
    else
    {
        target.draw (m_sprite, states);
    }

}

void Base::updateCurrent (sf::Time dt, CommandQueue& commands)
{
    updateTexts();

    if (isDestroyed())
    {
        m_explosion.update (dt);

        if (!m_explosionBegan)
        {
            SoundEffect::ID soundEffect = (randomInt (2) == 0) ?
                                          SoundEffect::Explosion1 : SoundEffect::Explosion2;
            playLocalSound (commands, soundEffect);

            //            if (!isAllied())
            //            {
            //                sf::Vector2f position = getWorldPosition();
            //
            //                Command command;
            //                command.category = Category::Network;
            //                command.action = derivedAction<NetworkNode>([position] (NetworkNode& node, sf::Time)
            //                {
            //                    node.notifyGameAction(GameActions::EnemyExplode, position);
            //                });
            //
            //                commands.push(command);
            //            }

            m_explosionBegan = true;
        }

        return;
    }

    Entity::updateCurrent (dt, commands);
}

void Base::playLocalSound (CommandQueue& commands,
                           SoundEffect::ID effect)
{
    sf::Vector2f worldPosition =  getWorldPosition();

    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode> (
                         [effect, worldPosition] (SoundNode & node, sf::Time)
    {
        node.playSound (effect, worldPosition);
    });

    commands.push (command);
}

void Base::updateTexts()
{
    if (isDestroyed())
    { m_healthDisplay->setString (""); }
    else
    { m_healthDisplay->setString (toString (getHitpoints()) + "HP"); }

    m_healthDisplay->setPosition (1000.f, 100.f);

}


void Base::checkNodePosition (const
                              std::vector<sf::FloatRect>
                              &virtualRectCollision
                              , std::multimap<int, SceneNode*>
                              &collisionListeToTest
                              , sf::Int32 nbCutX
                              , sf::Int32 nbCutY)
{
    /*
        Recalcule la position dans la grille de
        colllision si l'objet a bougé ou n'est pas
        initialisé
    */
    //    if (m_positionCollision == -9999)
    //    {
    int op (0);

    for (auto it = virtualRectCollision.cbegin()
                   ; it != virtualRectCollision.cend()
            ; ++it)
    {
        op += 1;

        // Regarde si la grille de collision contient le point. Plus optimisé que la fonction intersect.
        if (this->getBoundingRect().intersects (*it))
        {
            //               m_positionCollision = op;
            collisionListeToTest.insert (
                std::pair<int, SceneNode*> (op, this));

        }
    }

    //    }
}
