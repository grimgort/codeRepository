#ifndef EMITTERNODE_H
#define EMITTERNODE_H

#include <SceneNode.h>
#include <Particle.h>

class ParticleNode;

class EmitterNode : public SceneNode
{
public:
    explicit EmitterNode (Particle::Type type);

private:
    virtual void updateCurrent (sf::Time dt, CommandQueue& commands);

    void emitParticles (sf::Time dt);

private:
    sf::Time m_accumulatedTime;
    Particle::Type m_type;
    ParticleNode* m_particleSystem;
};

#endif // EMITTERNODE_H
