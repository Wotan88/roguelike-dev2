#include "entity.hpp"

game::level::Player::Player(Level* l) :
        AbstractEntity(l) {
    setProperty<string>("class", "Player");
    setProperty<int>("hp", 20);
}

game::level::Player::~Player() {

}

bool game::level::Player::onCollideTile(int x, int y) {
    AbstractTile* t = (*mLevel)(x, y);
    if (t && t->interactOnCollide(x, y, mLevel)) {
        return t->onInteract(x, y, mLevel, this);
    }

    return false;
}
