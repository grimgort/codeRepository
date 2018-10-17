#ifndef WORLD_H
#define WORLD_H

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.h>
#include <SceneNode.h>
#include <SpriteNode.h>
#include <Aircraft.h>
#include <CommandQueue.h>
#include <Command.h>
#include <Pickup.h>
#include <BloomEffect.h>
#include <SoundPlayer.h>
#include <NetworkProtocol.h>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>
// Forward declaration
namespace sf
{
class RenderTarget;
}

class NetworkNode;


class World : private sf::NonCopyable
{
public:
    explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds, bool networked = false);
    void update(sf::Time dt);
    void draw();

    sf::FloatRect getViewBounds() const;
    CommandQueue& getCommandQueue();
    Aircraft* addAircraft(int identifier);
    void removeAircraft(int identifier);
    void setCurrentBattleFieldPosition(float lineY);
    void setWorldHeight(float height);

    void addEnemy(Aircraft::Type type, float relX, float relY);
    void sortEnemies();

    bool hasAlivePlayer() const;
    bool hasPlayerReachedEnd() const;

    void setWorldScrollCompensation(float compensation);

    Aircraft* getAircraft(int identifier) const;
    sf::FloatRect getBattlefieldBounds() const;
    sf::FloatRect getWorldBounds() const;

    void createPickup(sf::Vector2f position, Pickup::Type type);
    bool pollGameAction(GameActions::Action& out);

private:
    void loadTextures();
    void adaptPlayerPosition();
    void adaptPlayerVelocity();
    void grilleDeCollision();
    void handleCollisions();
    void updateSounds();

    void buildScene();
    void addEnemies();
    void spawnEnemies();
    void destroyEntitiesOutsideWorld();
    void guideMissiles();

private:
    enum Layer
    {
        background,
        LowerAir,
        UpperAir,
        LayerCount
    };

    struct SpawnPoint
    {
        SpawnPoint(Aircraft::Type type, float x, float y):
            type(type),
            x(x),
            y(y)
        {
        }

        Aircraft::Type type;
        float x;
        float y;
    };


private:
    sf::RenderTarget& m_target;
    sf::RenderTexture m_sceneTexture;
    sf::View m_worldView;
    TextureHolder m_textures;
    FontHolder& m_fonts;
    SoundPlayer& m_sounds;

    SceneNode m_sceneGraph;
    //m_sceneLayers est un conteneur composé de sceneNode associé à un layer count.
    //Ici, il y a donc un sceneNode pour background, un autre pour Air, etc...
    std::array<SceneNode*,LayerCount> m_sceneLayers;
    CommandQueue m_commandQueue;

    sf::FloatRect m_worldBounds;
    sf::Vector2f m_spawnPosition;
    float m_scrollSpeed;
    float m_scrollSpeedCompensation;
    std::vector<Aircraft*>	m_playerAircrafts;

    std::vector<SpawnPoint> m_enemySpawnPoints;
    std::vector<Aircraft*> m_activeEnemies;

    BloomEffect m_bloomEffect;

    bool m_networkedWorld;
    NetworkNode* m_networkNode;
    SpriteNode* m_finishSprite;

    std::vector<sf::FloatRect> m_grilleDeCollision;
//public:
//    std::map<sf::Int32, SceneNode> m_collisionToCheck;
};

#endif // WORLD_H
