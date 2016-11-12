#include "level.hpp"
#include "registry.hpp"

#include <easylogging++.h>
#include <queue>

game::level::Level::Level(int width, int height) :
        mWidth(width), mHeight(height) {
    mData = vector<int>(width * height);
    mPresenceFlags = vector<int>(width * height);
    mDijkstraMap = vector<int>(width * height);
    mMetadata = vector<int>(width * height);
    mVisibility = vector<int>(width * height);
    mLastPlayerX = -1;
    mLastPlayerY = -1;
    mPlayer = nullptr;
    mUsx = mUsy = mDsx = mDsy = 0;
}

game::level::Level::~Level() {
    LOG(DEBUG)<< "Level destroyed";
    for (auto e: mEntities) {
        if (e->getProperty<string>("class", "") != "Player") {
            delete e;
        }
    }
}

int game::level::Level::getWidth() {
    return mWidth;
}

int game::level::Level::getHeight() {
    return mHeight;
}

game::level::AbstractTile* game::level::Level::operator ()(int x, int y) {
    if (x < 0 || y < 0 || x >= mWidth || y >= mHeight)
        return nullptr;
    int id = mData[x + y * mWidth];
    if (id) {
        return game::tileregistry::byId(id);
    } else {
        return nullptr;
    }
}

void game::level::Level::onPlayerDown(int x, int y) {
    mDsx = x;
    mDsy = y;
    std::remove_if(mEntities.begin(), mEntities.end(),
            [](AbstractEntity*& e)->bool {
                LOG(DEBUG)<< e->getProperty<string>("class", "");
                return false;
            });
}
void game::level::Level::onPlayerUp(int x, int y) {
    mUsx = x;
    mUsy = y;
    std::remove_if(mEntities.begin(), mEntities.end(),
            [](AbstractEntity*& e)->bool {
                LOG(DEBUG)<< e->getProperty<string>("class", "");
                return false;
            });
}

int game::level::Level::getTileVisibility(int x, int y) {
    return mVisibility[x + y * mWidth];
}

void game::level::Level::getDownExitPos(int& x, int& y) {
    x = mDsx;
    y = mDsy;
}

void game::level::Level::getUpExitPos(int& x, int& y) {
    x = mUsx;
    y = mUsy;
}

void game::level::Level::set(int x, int y, const string& name) {
    if (x < 0 || y < 0 || x >= mWidth || y >= mHeight)
        return;
    AbstractTile* t = game::tileregistry::byName(name);

    if (t) {
        mData[x + y * mWidth] = t->getId();
    } else {
        mData[x + y * mWidth] = 0;
    }
}

void game::level::Level::setMeta(int x, int y, int m) {
    mMetadata[x + y * mWidth] = m;
}

bool game::level::Level::isCollidable(int x, int y) {
    AbstractTile* t = operator ()(x, y);
    if (t) {
        return t->isCollidable(x, y, this);
    } else {
        return true;
    }
}

int game::level::Level::getMeta(int x, int y) {
    return mMetadata[x + y * mWidth];
}

const vector<game::level::AbstractEntity*>& game::level::Level::entities() {
    return mEntities;
}

game::level::AbstractEntity* game::level::Level::getEntityAt(int x, int y) {
    if (x < 0 || y < 0 || x >= mWidth || y >= mHeight)
        return nullptr;
    int f = mPresenceFlags[x + y * mWidth] & HAS_ENTITY_FLAG;
    if (!(f)) {
        return nullptr;
    } else {
        vector<AbstractEntity*>::iterator it = std::find_if(
                std::begin(mEntities), std::end(mEntities),
                [x, y](AbstractEntity*& e)->bool {
                    return (e && e->x() == x && e->y() == y);
                });
        if (it == mEntities.end())
            return nullptr;

        return *it;
    }
}

int game::level::Level::getPlayerDistance(int x, int y) {
    return mDijkstraMap[x + y * mWidth];
}

void game::level::Level::dijkstraGen(int ox, int oy, int v) {
    TIMED_FUNC(timerObj);
    mDijkstraMap[ox + oy * mWidth] = 0;
    std::queue<std::pair<int, int>> pq;
    pq.push( { ox, oy });
    while (!pq.empty()) {
        std::pair<int, int> xy = pq.front();
        pq.pop();
        for (int x = xy.first - 1; x <= xy.first + 1; x++) {
            for (int y = xy.second - 1; y <= xy.second + 1; y++) {
                game::level::AbstractTile* t = operator ()(x, y);
                if (!t)
                    continue;
                if (t->isCollidable(x, y, this))
                    continue;
                if (mDijkstraMap[x + y * mWidth] == 1000000) {
                    mDijkstraMap[x + y * mWidth] = mDijkstraMap[xy.first
                            + xy.second * mWidth] + 1;
                    pq.push( { x, y });
                }
            }
        }
    }
}

void game::level::Level::castFOVRay(int sx, int sy, float angle, int val) {
    int px, py;
    px = sx;
    py = sy;
    float rx, ry;
    rx = sx + 0.5;
    ry = sy + 0.5;
    float dx, dy;
    for (int i = 0; i < val; i++) {
        dx = std::cos(angle);
        dy = std::sin(angle);
        rx += dx;
        ry += dy;
        px = (int) rx;
        py = (int) ry;
        AbstractTile* t = (*this)(px, py);
        if (t) {
            if (t->isCollidable(px, py, this)) {
                mVisibility[px + py * mWidth] = 1;
                break;
            } else {
                mVisibility[px + py * mWidth] = 1;
                continue;
            }
        } else
            break;
    }
}

void game::level::Level::update() {
    std::replace_if(mVisibility.begin(), mVisibility.end(), [](int& v)-> bool {
        return v != 0;
    }, 2);
    int px, py;
    mPlayer->getPosition(px, py);
    for (int i = 0; i < 360; i++) {
        castFOVRay(px, py, i, 10);
    }
    {
        int px, py;
        mPlayer->getPosition(px, py);
        std::fill(mDijkstraMap.begin(), mDijkstraMap.end(), 1000000);

        dijkstraGen(px, py, 0);
    }
}

void game::level::Level::addEntity(AbstractEntity* e) {
    if (!e)
        return;

    LOG(DEBUG)<< "Adding entity "<<e;
    int x, y;
    e->getPosition(x, y);
    LOG(DEBUG)<< x << ", " << y;
    mPresenceFlags[x + y * mWidth] |= HAS_ENTITY_FLAG;

    LOG(DEBUG)<< mPresenceFlags[x + y * mWidth];

    if (e->getProperty<string>("class", "") == "Player")
        mPlayer = (Player*) e;

    e->setLevel(this);
    mEntities.push_back(e);
}

void game::level::Level::spawn(int x, int y, const string& name) {
    AbstractEntity* e = game::entityregistry::byName(name);
    if (e) {
        e->setPosition(x, y);
        addEntity(e);
    }
}

void game::level::Level::onEntityMoved(int sx, int sy, int dx, int dy) {
    if (sx >= 0 && sy >= 0 && sx < mWidth && sy < mHeight
            && (mPresenceFlags[sx + sy * mWidth] & HAS_ENTITY_FLAG)) {
        mPresenceFlags[sx + sy * mWidth] ^= HAS_ENTITY_FLAG;
    }
    if (dx >= 0 && dy >= 0 && dx < mWidth && dy < mHeight) {
        mPresenceFlags[dx + dy * mWidth] |= HAS_ENTITY_FLAG;
    }
}
