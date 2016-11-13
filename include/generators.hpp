#ifndef INCLUDE_GENERATORS_HPP_
#define INCLUDE_GENERATORS_HPP_
#include <string>
#include "level.hpp"

namespace game {
namespace level {
namespace gen {

// Produces simple flat level
class BoxGenerator: public AbstractGenerator {
public:
    // Constructor
    BoxGenerator();
    // Destructor
    virtual ~BoxGenerator();

    void generate(Level* l) override;
private:
    string mFloorTile, mWallTile;
};

struct Rect {
    int x, y;
    int width, height;
};

// Produces dungeon-like level
class DungeonGenerator: public AbstractGenerator {
public:
    enum Tile {
        Unused = ' ',
        Floor = '.',
        Corridor = ',',
        Wall = '#',
        ClosedDoor = '+',
        OpenDoor = '-',
        UpStairs = '<',
        DownStairs = '>'
    };

    enum Direction {
        North, South, West, East, DirectionCount
    };

    DungeonGenerator();
    virtual ~DungeonGenerator();

    void generate(Level* l) override;
    void getSpawnPosition(int& x, int& y) override;
private:
    char getTile(int x, int y) const;
    void setTile(int x, int y, char tile);
    bool createFeature();
    bool createFeature(int x, int y, Direction dir);
    bool makeRoom(int x, int y, Direction dir, bool firstRoom = false);
    bool makeCorridor(int x, int y, Direction dir);
    bool placeRect(const Rect& rect, char tile);
    bool placeObject(char tile);

    int mWidth, mHeight;
    std::vector<char> mTiles;
    std::vector<Rect> mRooms;
    std::vector<Rect> mExits;
    int mPsx, mPsy;
};

}
}
}

#endif
