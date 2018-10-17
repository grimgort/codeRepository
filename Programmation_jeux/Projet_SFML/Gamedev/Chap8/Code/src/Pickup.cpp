#include "Pickup.h"
#include <DataTables.h>
#include <Category.h>
#include <CommandQueue.h>
#include <Utility.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
    const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures)
   : Entity(1)
   , m_type(type)
   , m_sprite(textures.get(Table[type].texture), Table[type].textureRect)
{
    centerOrigin(m_sprite);
}

unsigned int Pickup::getCategory() const
{
    return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
    return getWorldTransform().transformRect(m_sprite.getGlobalBounds());
}

void Pickup::apply(Aircraft& player) const
{
    Table[m_type].action(player);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}






