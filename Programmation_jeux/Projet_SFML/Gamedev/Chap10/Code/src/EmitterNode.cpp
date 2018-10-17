#include "EmitterNode.h"
#include <ParticleNode.h>
#include <CommandQueue.h>
#include <Command.h>


EmitterNode::EmitterNode(Particle::Type type)
: SceneNode()
, m_accumulatedTime(sf::Time::Zero)
, m_type(type)
, m_particleSystem(nullptr)
{
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (m_particleSystem)
    {
        emitParticles(dt);
    }
    else
    {
        auto finder = [this] (ParticleNode& container, sf::Time)
        {
            if (container.getParticleType() == m_type)
                m_particleSystem = &container;
        };

        Command command;
        command.category = Category::ParticleSystem;
        command.action = derivedAction<ParticleNode>(finder);

        commands.push(command);
    }
}

void EmitterNode::emitParticles(sf::Time dt)
{
    const float emissionRate = 30.f;
    const sf::Time interval = sf::seconds(1.f) / emissionRate;

    m_accumulatedTime += dt;

    while (m_accumulatedTime > interval)
    {
        m_accumulatedTime -= interval;
        m_particleSystem->addParticle(getWorldPosition());
    }
}




