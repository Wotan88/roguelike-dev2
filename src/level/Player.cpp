#include "entity.hpp"
#include "messages.hpp"

/*
 * 0 - Left hand
 * 1 - Right hand
 * 2 - Head
 * 3 - Body
 * 4 - Legs
 * 5 - Boots
 * 6 - Ring 1
 * 7 - Ring 2
 * 8 - Missile weapon
 * 9 - ???
 */

game::level::Player::Player(Level* l) :
        AbstractEntity(l), InventoryHolder(10), EquipmentHolder(10) {
    setProperty<string>("class", "Player");
    setProperty<int>("hp", 20);

    // Equipment slots
    setEquipmentSlotName(0, "Left hand");
    setEquipmentSlotName(1, "Right hand");
    setEquipmentSlotName(2, "Headgear");
    setEquipmentSlotName(3, "Body armor");
    setEquipmentSlotName(4, "Leg armor");
    setEquipmentSlotName(5, "Boots");
    setEquipmentSlotName(6, "Ring");
    setEquipmentSlotName(7, "Ring");
    setEquipmentSlotName(8, "Missile weapon");
    setEquipmentSlotName(9, "Unnamed slot");

    missileAmmoCount = 0;
    missileAmmoItem = nullptr;
}

game::level::Player::~Player() {

}

bool game::level::Player::hasMissileAmmo() {
    return missileAmmoCount && missileAmmoItem;
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
