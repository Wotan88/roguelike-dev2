#include "items/meleeWeapon.hpp"
#include "dice.hpp"

game::item::MeleeWeapon::MeleeWeapon() {
    setProperty<int>("equipSlot", 0);
}

game::item::MeleeWeapon::~MeleeWeapon() {
}

bool game::item::MeleeWeapon::canAttackWith() {
    return true;
}

bool game::item::MeleeWeapon::onAttackWith(level::AbstractEntity* src,
        level::AbstractEntity* target) {
    int dmgMod = src->getAttribute("STR");
    int dmgX = getProperty<int>("dx", 0);
    int dmgY = getProperty<int>("dy", 0);
    int dmg = dice::roll(dmgX, dmgY) + dmgMod;
    if (dmg) {
        target->onAttackedBy(dmg, src);
    }
    return true;
}

bool game::item::MeleeWeapon::isEquipable() {
    return true;
}
