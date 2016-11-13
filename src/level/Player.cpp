#include "entity.hpp"
#include "messages.hpp"

game::level::Player::Player(Level* l) :
        AbstractEntity(l), InventoryHolder(10) {
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

void game::level::Player::onAttackedBy(int dmg, AbstractEntity* src) {
    if (src) {
        messages::push(
                "You were attacked by "
                        + src->getProperty<string>("name", "unknown entity"));
    }
    messages::push("(-" + std::to_string(dmg) + " HP)");
}
