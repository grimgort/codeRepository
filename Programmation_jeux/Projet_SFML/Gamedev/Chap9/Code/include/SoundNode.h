#ifndef SOUNDNODE_H
#define SOUNDNODE_H

#include <SceneNode.h>
#include <ResourceIdentifiers.h>

class SoundPlayer;

class SoundNode : public SceneNode
{
    public:
        explicit SoundNode(SoundPlayer& player);
        void playSound(SoundEffect::ID sound, sf::Vector2f position);

        virtual unsigned int getCategory() const;

    private:
        SoundPlayer& m_sounds;
};

#endif // SOUNDNODE_H
