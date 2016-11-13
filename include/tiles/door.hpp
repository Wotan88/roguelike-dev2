#ifndef INCLUDE_TILES_DOOR_HPP_
#define INCLUDE_TILES_DOOR_HPP_
#include "level.hpp"

namespace game {
namespace level {
namespace tile {

// Generic door
class Door: public game::level::AbstractTile {
public:
    Door() :
            AbstractTile() {
    }
    virtual ~Door() {
    }

    bool isCollidable(int x, int y, Level* l) override;
    bool interactOnCollide(int x, int y, Level* l) override;
    bool onInteract(int x, int y, Level* l, AbstractEntity* src) override;
    int getIconIndex(int x, int y, Level* l) override;
};

}
}
}

#endif
