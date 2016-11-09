#include "generators.hpp"

#include <pcg_random.hpp>
#include <pcg_extras.hpp>
#include <random>

static pcg_extras::seed_seq_from<std::random_device> seed_source;
static pcg32 rng = pcg32(seed_source);

int randomInt(int exclusiveMax) {
    std::uniform_int_distribution<> dist(0, exclusiveMax - 1);
    return dist(rng);
}

int randomInt(int min, int max) {
    std::uniform_int_distribution<> dist(0, max - min);
    return dist(rng) + min;
}

bool randomBool(double probability = 0.5) {
    std::bernoulli_distribution dist(probability);
    return dist(rng);
}

game::level::gen::DungeonGenerator::DungeonGenerator() {
    mWidth = 0;
    mHeight = 0;
}

game::level::gen::DungeonGenerator::~DungeonGenerator() {

}

char game::level::gen::DungeonGenerator::getTile(int x, int y) const {
    if (x < 0 || y < 0 || x >= mWidth || y >= mHeight)
        return Unused;

    return mTiles[x + y * mWidth];
}

void game::level::gen::DungeonGenerator::setTile(int x, int y, char t) {
    mTiles[x + y * mWidth] = t;
}

bool game::level::gen::DungeonGenerator::makeRoom(int x, int y, Direction dir,
        bool firstRoom) {
    static const int minRoomSize = 5;
    static const int maxRoomSize = 20;

    Rect room;
    room.width = randomInt(minRoomSize, maxRoomSize);
    room.height = randomInt(minRoomSize, maxRoomSize);

    if (dir == North) {
        room.x = x - room.width / 2;
        room.y = y - room.height;
    }

    else if (dir == South) {
        room.x = x - room.width / 2;
        room.y = y + 1;
    }

    else if (dir == West) {
        room.x = x - room.width;
        room.y = y - room.height / 2;
    }

    else if (dir == East) {
        room.x = x + 1;
        room.y = y - room.height / 2;
    }

    if (placeRect(room, Floor)) {
        mRooms.emplace_back(room);

        if (dir != South || firstRoom)
            mExits.emplace_back(Rect { room.x, room.y - 1, room.width, 1 });
        if (dir != North || firstRoom)
            mExits.emplace_back(Rect { room.x, room.y + room.height, room.width,
                    1 });
        if (dir != East || firstRoom)
            mExits.emplace_back(Rect { room.x - 1, room.y, 1, room.height });
        if (dir != West || firstRoom)
            mExits.emplace_back(
                    Rect { room.x + room.width, room.y, 1, room.height });

        return true;
    }

    return false;
}

bool game::level::gen::DungeonGenerator::createFeature(int x, int y,
        Direction dir) {
    static const int roomChance = 50;

    int dx = 0;
    int dy = 0;

    if (dir == North)
        dy = 1;
    else if (dir == South)
        dy = -1;
    else if (dir == West)
        dx = 1;
    else if (dir == East)
        dx = -1;

    if (getTile(x + dx, y + dy) != Floor && getTile(x + dx, y + dy) != Corridor)
        return false;

    if (randomInt(100) < roomChance) {
        if (makeRoom(x, y, dir)) {
            setTile(x, y, ClosedDoor);

            return true;
        }
    }

    else {
        if (makeCorridor(x, y, dir)) {
            if (getTile(x + dx, y + dy) == Floor)
                setTile(x, y, ClosedDoor);
            else

                setTile(x, y, Corridor);

            return true;
        }
    }

    return false;
}

bool game::level::gen::DungeonGenerator::createFeature() {
    for (int i = 0; i < 1000; ++i) {
        if (mExits.empty())
            break;

        int r = randomInt(mExits.size());
        int x = randomInt(mExits[r].x, mExits[r].x + mExits[r].width - 1);
        int y = randomInt(mExits[r].y, mExits[r].y + mExits[r].height - 1);

        for (int j = 0; j < DirectionCount; ++j) {
            if (createFeature(x, y, static_cast<Direction>(j))) {
                mExits.erase(mExits.begin() + r);
                return true;
            }
        }
    }

    return false;
}

bool game::level::gen::DungeonGenerator::placeRect(const Rect& rect,
        char tile) {
    if (rect.x < 1 || rect.y < 1 || rect.x + rect.width > mWidth - 1
            || rect.y + rect.height > mHeight - 1) {
        return false;
    }

    for (int y = rect.y; y < rect.y + rect.height; ++y)
        for (int x = rect.x; x < rect.x + rect.width; ++x) {
            if (getTile(x, y) != Unused) {
                return false;
            }
        }

    for (int y = rect.y - 1; y < rect.y + rect.height + 1; ++y)
        for (int x = rect.x - 1; x < rect.x + rect.width + 1; ++x) {
            if (x == rect.x - 1 || y == rect.y - 1 || x == rect.x + rect.width
                    || y == rect.y + rect.height)
                setTile(x, y, Wall);
            else
                setTile(x, y, tile);
        }

    return true;
}

bool game::level::gen::DungeonGenerator::placeObject(char tile) {
    if (mRooms.empty())
        return false;

    int r = randomInt(mRooms.size());
    int x = randomInt(mRooms[r].x + 1, mRooms[r].x + mRooms[r].width - 2);
    int y = randomInt(mRooms[r].y + 1, mRooms[r].y + mRooms[r].height - 2);

    if (getTile(x, y) == Floor) {
        setTile(x, y, tile);

        mRooms.erase(mRooms.begin() + r);

        return true;
    }

    return false;
}

bool game::level::gen::DungeonGenerator::makeCorridor(int x, int y,
        Direction dir) {
    static const int minCorridorLength = 3;
    static const int maxCorridorLength = 6;

    Rect corridor;
    corridor.x = x;
    corridor.y = y;

    if (randomBool()) {
        corridor.width = randomInt(minCorridorLength, maxCorridorLength);
        corridor.height = 1;

        if (dir == North) {
            corridor.y = y - 1;

            if (randomBool())
                corridor.x = x - corridor.width + 1;
        }

        else if (dir == South) {
            corridor.y = y + 1;

            if (randomBool())
                corridor.x = x - corridor.width + 1;
        }

        else if (dir == West)
            corridor.x = x - corridor.width;

        else if (dir == East)
            corridor.x = x + 1;
    }

    else {
        corridor.width = 1;
        corridor.height = randomInt(minCorridorLength, maxCorridorLength);

        if (dir == North)
            corridor.y = y - corridor.height;

        else if (dir == South)
            corridor.y = y + 1;

        else if (dir == West) {
            corridor.x = x - 1;

            if (randomBool())
                corridor.y = y - corridor.height + 1;
        }

        else if (dir == East) {
            corridor.x = x + 1;

            if (randomBool())
                corridor.y = y - corridor.height + 1;
        }
    }

    if (placeRect(corridor, Corridor)) {
        if (dir != South && corridor.width != 1)
            mExits.emplace_back(
                    Rect { corridor.x, corridor.y - 1, corridor.width, 1 });
        if (dir != North && corridor.width != 1)
            mExits.emplace_back(Rect { corridor.x, corridor.y + corridor.height,
                    corridor.width, 1 });
        if (dir != East && corridor.height != 1)
            mExits.emplace_back(
                    Rect { corridor.x - 1, corridor.y, 1, corridor.height });
        if (dir != West && corridor.height != 1)
            mExits.emplace_back(Rect { corridor.x + corridor.width, corridor.y,
                    1, corridor.height });

        return true;
    }

    return false;
}

void game::level::gen::DungeonGenerator::generate(std::shared_ptr<Level>& l) {
    mWidth = l->getWidth();
    mHeight = l->getHeight();
    mTiles.reserve(mWidth * mHeight);
    for (int i = 0; i < mWidth * mHeight; i++) {
        mTiles[i] = Unused;
    }

    if (!makeRoom(mWidth / 2, mHeight / 2,
            static_cast<Direction>(randomInt(4), true))) {
        return;
    }

    for (int i = 1; i < 50; ++i) {
        if (!createFeature()) {
            break;
        }
    }

    if (!placeObject(UpStairs)) {
        return;
    }

    if (!placeObject(DownStairs)) {
        return;
    }

    for (char& tile : mTiles) {
        if (tile == Unused)
            tile = '.';
        else if (tile == Floor || tile == Corridor)
            tile = ' ';
    }

    string floorTile = getProperty<string>("floor", "");
    string wallTile = getProperty<string>("wall", "");
    string doorTile = getProperty<string>("door", "");

    for (int y = 0; y < mHeight; y++) {
        for (int x = 0; x < mWidth; x++) {
            int t = mTiles[x + y * mWidth];
            if (t == '.' || t == ',') {
                l->set(x, y, floorTile);
                continue;
            }
            if (t == '#') {
                l->set(x, y, wallTile);
                continue;
            }
            if (t == '-' || t == '+') {
                l->set(x, y, doorTile);
                continue;
            }
        }
    }
}
