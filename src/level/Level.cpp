#include "level.hpp"
#include "registry.hpp"

#include <easylogging++.h>

game::level::Level::Level(int width, int height) :
        mWidth(width), mHeight(height) {
    mData = vector<int>(width * height);
}

game::level::Level::~Level() {

}

int game::level::Level::getWidth() {
    return mWidth;
}

int game::level::Level::getHeight() {
    return mHeight;
}

std::weak_ptr<game::level::AbstractTile> game::level::Level::operator ()(int x,
        int y) {
    static std::weak_ptr<AbstractTile> null_tile_ptr;
    if (x < 0 || y < 0 || x >= mWidth || y >= mHeight)
        return null_tile_ptr;
    int id = mData[x + y * mWidth];
    if (id) {
        return std::weak_ptr<AbstractTile> { game::tileregistry::byId(id) };
    } else {
        return null_tile_ptr;
    }
}

void game::level::Level::set(int x, int y, const string& name) {
    if (x < 0 || y < 0 || x >= mWidth || y >= mHeight)
        return;
    std::weak_ptr<AbstractTile> t = game::tileregistry::byName(name);

    if (auto p = t.lock()) {
        mData[x + y * mWidth] = p->getId();
    } else {
        mData[x + y * mWidth] = 0;
    }
}
