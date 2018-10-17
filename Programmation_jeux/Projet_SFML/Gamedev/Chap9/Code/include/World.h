#ifndef WORLD_H
#define WORLD_H

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.h>
#include <SceneNode.h>
#include <SpriteNode.h>
#include <Aircraft.h>
#include <CommandQueue.h>
#include <Command.h>
#include <BloomEffect.h>
#include <SoundPlayer.h>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>
// Forward declaration
namespace sf
{
	class RenderWindow;
}


class World : private sf::NonCopyable
{
    public:
        explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds);
        void update(sf::Time dt);
        void draw();

        CommandQueue& getCommandQueue();

        bool hasAlivePlayer() const;
        bool hasPlayerReachedEnd() const;


    private:
        void loadTextures();
        void adaptPlayerPosition();
        void adaptPlayerVelocity();
        void handleCollisions();
        void updateSounds();

        void buildScene();
        void addEnemies();
        void addEnemy(Aircraft::Type type, float relX, float relY);
        void spawnEnemies();
        void destroyEntitiesOutsideView();
        void guideMissiles();
        sf::FloatRect getViewBounds() const;
        sf::FloatRect getBattlefieldBounds() const;


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
        Aircraft* m_playerAircraft;

        std::vector<SpawnPoint> m_enemySpawnPoints;
        std::vector<Aircraft*> m_activeEnemies;

        BloomEffect m_bloomEffect;
};

#endif // WORLD_H
