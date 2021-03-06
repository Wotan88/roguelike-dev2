#ifndef INCLUDE_LEVEL_HPP_
#define INCLUDE_LEVEL_HPP_
#include "core.hpp"
#include "structs.hpp"
#include "inventory.hpp"

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace game {
namespace level {

class AbstractEntity;
class Level;
class Player;

namespace depths {
Level* get(int d);
void push(Level* l);
Level* last();
int count();
}

// Abstract Tile class
class AbstractTile: public game::data::PropertyHolder {
public:
    // Constructor
    AbstractTile();
    // Destructor
    virtual ~AbstractTile();

    // Getters
    int getId();
    virtual bool isCollidable(int x, int y, Level* l);
    virtual bool onInteract(int x, int y, Level* l, AbstractEntity* src);
    virtual bool interactOnCollide(int x, int y, Level* l);
    virtual bool isDownstairs(int x, int y, Level* l);
    virtual bool isUpstairs(int x, int y, Level* l);

    // Setters
    void setId(int newId);

    virtual int getBackgroundColor(int x, int y, Level* l);
    virtual int getForegroundColor(int x, int y, Level* l);
    virtual int getIconIndex(int x, int y, Level* l);
private:
    int mId;
};

// Level class
class Level: public game::data::PropertyHolder {
public:
    // Constructor
    Level(int width, int height);
    // Destructor
    ~Level();
    // Getters
    int getWidth();
    int getHeight();
    // Returns struct with information about tile
    AbstractTile* operator()(int x, int y);
    int getMeta(int x, int y);
    bool isCollidable(int x, int y);
    int getPlayerDistance(int x, int y);
    AbstractEntity* getEntityAt(int x, int y);
    void getDownExitPos(int& x, int& y);
    void getUpExitPos(int& x, int& y);
    int getTileVisibility(int x, int y);
    int getTileId(int x, int y);
    // Sets tile
    void set(int x, int y, const string& name);
    void setMeta(int x, int y, int m);
    void spawn(int x, int y, const string& name);
    void addEntity(AbstractEntity* e);
    void removeEntity(AbstractEntity* e);

    void update();
    void onPlayerUp(int x, int y);
    void onPlayerDown(int x, int y);
    void onEntityMoved(int sx, int sy, int dx, int dy);

    item::InventoryItem* getLootAt(int x, int y);
    void newLootAt(int x, int y, const string& name, int cnt = 1);
    void setLootAt(int x, int y, item::AbstractItem* i, int cnt);
    void updateLootCount(int x, int y, int cnt);

    const vector<game::level::AbstractEntity*>& entities();

    static constexpr int HAS_ENTITY_FLAG = 1;
private:
    void dijkstraGen(int x, int y, int v);
    void castFOVRay(int sx, int sy, float angle, int val);

    int mDsx, mDsy;
    int mUsx, mUsy;
    // Size
    int mWidth, mHeight;
    // Tile IDs
    vector<int> mData;
    // Entities
    vector<AbstractEntity*> mEntities;
    // Object presence flags
    vector<int> mPresenceFlags;
    vector<item::InventoryItem*> mLoot;
    // Dijkstra map (distances from player to point)
    vector<int> mDijkstraMap;
    vector<int> mMetadata;
    Player* mPlayer;
    int mLastPlayerX, mLastPlayerY;
    vector<int> mVisibility;
};

// Abstract generator interface
class AbstractGenerator: public game::data::PropertyHolder {
public:
    // Constructor
    AbstractGenerator();
    // Destructor
    virtual ~AbstractGenerator();

    // Main function
    virtual void generate(game::level::Level* l);

    virtual void getSpawnPosition(int& x, int& y);
};

}
}

#endif
