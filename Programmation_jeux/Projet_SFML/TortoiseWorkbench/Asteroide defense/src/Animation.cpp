#include "Animation.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


Animation::Animation()
    : m_sprite()
    , m_frameSize()
    , m_numFrames (0)
    , m_currentFrame (0)
    , m_duration (sf::Time::Zero)
    , m_elapsedTime (sf::Time::Zero)
    , m_repeat (false)
{
}

Animation::Animation (const sf::Texture& texture)
    : m_sprite (texture)
    , m_frameSize()
    , m_numFrames (0)
    , m_currentFrame (0)
    , m_duration (sf::Time::Zero)
    , m_elapsedTime (sf::Time::Zero)
    , m_repeat (false)
{
}

void Animation::setTexture (const sf::Texture& texture)
{
    m_sprite.setTexture (texture);
}

const sf::Texture* Animation::getTexture() const
{
    return m_sprite.getTexture();
}

void Animation::setFrameSize (sf::Vector2i frameSize)
{
    m_frameSize = frameSize;
}

sf::Vector2i Animation::getFrameSize() const
{
    return m_frameSize;
}

void Animation::setNumFrames (std::size_t numFrames)
{
    m_numFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
    return m_numFrames;
}

void Animation::setDuration (sf::Time duration)
{
    m_duration = duration;
}

sf::Time Animation::getDuration() const
{
    return m_duration;
}

void Animation::setRepeating (bool flag)
{
    m_repeat =  flag;
}

bool Animation::isRepeating() const
{
    return m_repeat;
}

void Animation::restart()
{
    m_currentFrame = 0;
}

bool Animation::isFinished() const
{
    return m_currentFrame >= m_numFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
    return sf::FloatRect (getOrigin(),
                          static_cast<sf::Vector2f> (getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
    return getTransform().transformRect (getLocalBounds());
}

void Animation::update (sf::Time dt)
{
    sf::Time timePerFrame = m_duration / static_cast<float> (m_numFrames);
    m_elapsedTime += dt;

    sf::Vector2i textureBounds (m_sprite.getTexture()->getSize());
    sf::IntRect textureRect = m_sprite.getTextureRect();

    if (m_currentFrame == 0)
    { textureRect = sf::IntRect (0, 0, m_frameSize.x, m_frameSize.y); }

    while (m_elapsedTime >= timePerFrame
            && (m_currentFrame <= m_numFrames || m_repeat))
    {
        textureRect.left += textureRect.width;

        if (textureRect.left + textureRect.width > textureBounds.x)
        {
            textureRect.left = 0;
            textureRect.top += textureRect.height;
        }

        m_elapsedTime -= timePerFrame;

        if (m_repeat)
        {
            m_currentFrame = (m_currentFrame + 1) % m_numFrames;

            if (m_currentFrame == 0)
            { textureRect = sf::IntRect (0, 0, m_frameSize.x, m_frameSize.y); }
        }
        else
        {
            m_currentFrame++;
        }
    }

    m_sprite.setTextureRect (textureRect);
}

void Animation::draw (sf::RenderTarget& target,
                      sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw (m_sprite, states);
}
