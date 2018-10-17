#include "SoundPlayer.h"

#include <SFML/Audio/Listener.hpp>

#include <cmath>

namespace
{
const float ListenerZ = 300.f;
const float Attenuation = 8.f;
const float minDistance2D = 200.f;
const float minDistance3D = std::sqrt (minDistance2D * minDistance2D +
                                       ListenerZ*ListenerZ);
}

SoundPlayer::SoundPlayer()
    : m_soundBuffers()
    , m_sounds()
{
    m_soundBuffers.load (SoundEffect::AlliedGunfire,
                         "media/Sound/AlliedGunfire.wav");
    m_soundBuffers.load (SoundEffect::EnemyGunfire,
                         "media/Sound/EnemyGunfire.wav");
    m_soundBuffers.load (SoundEffect::Explosion1,
                         "media/Sound/Explosion1.wav");
    m_soundBuffers.load (SoundEffect::Explosion2,
                         "media/Sound/Explosion2.wav");
    m_soundBuffers.load (SoundEffect::LaunchMissile,
                         "media/Sound/LaunchMissile.wav");
    m_soundBuffers.load (SoundEffect::CollectPickup,
                         "media/Sound/CollectPickup.wav");
    m_soundBuffers.load (SoundEffect::Button,
                         "media/Sound/Button.wav");
    m_soundBuffers.load (SoundEffect::LaunchAsteroideUn,
                         "media/Sound/LaunchMissile.wav");

    sf::Listener::setDirection (0.f, 0.f, -1.f);
}

void SoundPlayer::play (SoundEffect::ID effect)
{
    play (effect, getListenerPosition());
}

void SoundPlayer::play (SoundEffect::ID effect, sf::Vector2f position)
{
    m_sounds.push_back (sf::Sound());
    sf::Sound& sound = m_sounds.back();

    sound.setBuffer (m_soundBuffers.get (effect));
    sound.setPosition (position.x, -position.y, 0.f);
    sound.setAttenuation (Attenuation);
    sound.setMinDistance (minDistance3D);

    sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
    m_sounds.remove_if ([] (const sf::Sound & s)
    {
        return s.getStatus() == sf::Sound::Stopped;
    });
}

void SoundPlayer::setListenerPosition (sf::Vector2f position)
{
    sf::Listener::setPosition (position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    sf::Vector3f position = sf::Listener::getPosition();
    return sf::Vector2f (position.x, -position.y);
}
