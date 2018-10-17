#include "SoundNode.h"
#include <soundPlayer.h>

SoundNode::SoundNode(SoundPlayer& player)
    : SceneNode()
    , m_sounds(player)
{
}

void SoundNode::playSound(SoundEffect::ID sound, sf::Vector2f position)
{
    m_sounds.play(sound, position);
}

unsigned int SoundNode::getCategory() const
{
    return Category::SoundEffect;
}
