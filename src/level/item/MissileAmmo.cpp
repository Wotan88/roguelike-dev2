#include "items/missileAmmo.hpp"
#include "messages.hpp"
#include "dice.hpp"

game::item::MissileAmmo::MissileAmmo() {
    setProperty<string>("class", "MissileAmmo");
}

game::item::MissileAmmo::~MissileAmmo() {

}

bool game::item::MissileAmmo::isEquipable() {
    return true;
}

bool game::item::MissileAmmo::onHitEntity(level::AbstractEntity* src,
        level::AbstractEntity* target, int weaponBonus) {
    int dx = getProperty<int>("dx", 0);
    int dy = getProperty<int>("dy", 0);
    int dmg = dice::roll(dx, dy);

    if (dmg) {
        if (src && src->getProperty<string>("class", "") == "Player") {
            messages::push(
                    "Your projectile hits "
                            + target->getProperty<string>("name", "")
                            + " and damages it");
        }
        target->onAttackedBy(dmg, src);
    } else {
        if (src && src->getProperty<string>("class", "") == "Player") {
            messages::push("You don't manage to damage target");
        }
    }
    return true;
}
