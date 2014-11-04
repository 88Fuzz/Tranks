#ifndef __WORLD_HPP__
#define __WORLD_HPP__

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "Player.hpp"
#include "MapCreator.hpp"
#include "Container.hpp"
#include "ButtonTypes.hpp"
#include "CommandQueue.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <queue>

// Forward declaration
namespace sf
{
    class RenderWindow;
}

class World: private sf::NonCopyable
{
public:
    World(sf::RenderWindow*, FontHolder*);
    virtual ~World();
    void update(sf::Time);
    void draw();
    void handleEvent(const sf::Event*);

//    CommandQueue* getCommandQueue();

private:
    enum Layer
    {
        Background,
        LowerAir,
        UpperAir,
        LAYERCOUNT
    };

    //TODO change this to the number of players
    int numPlayers;
    int playersReady;
    int mapTileWidth;
    int mapTileHeight;

    sf::RenderWindow* window;
    sf::RenderTexture sceneTexture;
    sf::View worldView;
    TextureHolder textures;
    FontHolder* fonts;
    std::vector<Player *> players;
    std::vector<GUI::TRANK_CONTROLS> pendingPlayerCommands;
    BoardPiece* map;

    SceneNode sceneGraph;
    SceneNode * sceneLayers[];
    CommandQueue commandQueue;

    sf::FloatRect worldBounds;
    GUI::Container trankControls;
    GUI::Container sendCommandBox;

    void loadTextures();
    void adaptPlayerPosition();
    void adaptPlayerVelocity();

    void buildScene();
    void validateMoves();
    void queueActions();
//    void addEnemies();
//    void addEnemy(Aircraft::Type type, float relX, float relY);
//    void spawnEnemies();
//    void destroyEntitiesOutsideView();
//    void guideMissiles();
    sf::FloatRect getViewBounds() const;
    sf::FloatRect getBattlefieldBounds() const;
    bool validateAction(Player *, int);
};

#endif
