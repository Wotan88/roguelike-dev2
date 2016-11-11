#include "tiles/door.hpp"

bool game::level::tile::Door::isCollidable(int x, int y, Level* l) {
    return l->getMeta(x, y) == 0;
}

bool game::level::tile::Door::interactOnCollide(int x, int y, Level* l) {
    return true;
}

bool game::level::tile::Door::onInteract(int x, int y, Level* l,
        AbstractEntity* src) {
    l->setMeta(x, y, 1 - l->getMeta(x, y));
    return true;
}

int game::level::tile::Door::getIconIndex(int x, int y, Level* l){
    if (l->getMeta(x, y) == 0){
        return getProperty<int>("iconClosed", '+');
    } else {
        return getProperty<int>("iconOpen", '-');
    }
}
