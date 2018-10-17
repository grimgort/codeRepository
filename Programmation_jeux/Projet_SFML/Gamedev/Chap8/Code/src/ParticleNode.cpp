#include "ParticleNode.h"
#include <Foreach.hpp>
#include <DataTables.h>
#include <ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <algorithm>


namespace
{
    const std::vector<ParticleData> Table = initializeParticleData();
}

ParticleNode::ParticleNode(Particle::Type type, const TextureHolder& textures)
: SceneNode()
, m_particles()
, m_texture(textures.get(Textures::Particle))
, m_type(type)
, m_vertexArray(sf::Quads)
, m_needsVertexUpdate(true)
{
}

void ParticleNode::addParticle(sf::Vector2f position)
{
    Particle particle;
    particle.position = position;
    particle.color = Table[m_type].color;
    particle.lifetime = Table[m_type].lifetime;

    m_particles.push_back(particle);
}
Particle::Type ParticleNode::getParticleType() const
{
    return m_type;
}

unsigned int ParticleNode::getCategory() const
{
    return Category::ParticleSystem;
}

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue&)
{
    while (!m_particles.empty() && m_particles.front().lifetime <= sf::Time::Zero)
        m_particles.pop_front();

    FOREACH(Particle& particle, m_particles)
    particle.lifetime -= dt;
    m_needsVertexUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_needsVertexUpdate)
    {
        computeVertices();
        m_needsVertexUpdate = false;
    }

    states.texture = &m_texture;

    target.draw(m_vertexArray, states);
}

void ParticleNode::addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
{
    sf::Vertex vertex;
    vertex.position =  sf::Vector2f(worldX, worldY);
    vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
    vertex.color = color;

    m_vertexArray.append(vertex);
}

void ParticleNode::computeVertices() const
{
    sf::Vector2f size(m_texture.getSize());
    sf::Vector2f half = size /2.f;

    m_vertexArray.clear();
    FOREACH(const Particle& particle, m_particles)
    {
        sf::Vector2f pos = particle.position;
        sf::Color color = particle.color;

        float ratio = particle.lifetime.asSeconds() / Table[m_type].lifetime.asSeconds();
        color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

        addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
        addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
        addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
        addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
    }
}
