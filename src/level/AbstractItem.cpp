#include "item.hpp"
#include "serialization.hpp"

game::item::AbstractItem::AbstractItem() {
}

game::item::AbstractItem::~AbstractItem() {
}

bool game::item::AbstractItem::onUsed(level::AbstractEntity* src) {
    return false;
}

bool game::item::AbstractItem::isEquipable() {
    return getProperty<bool>("equipable", false);
}

bool game::item::AbstractItem::isUsable() {
    return getProperty<bool>("usable", false);
}

bool game::item::AbstractItem::canAttackWith() {
    return false;
}

bool game::item::AbstractItem::onAttackWith(level::AbstractEntity* src,
        level::AbstractEntity* target) {
    return false;
}

game::item::AbstractItem* game::item::AbstractItem::clone() {
    LOG(DEBUG)<< "Clone() " << getProperty<string>("assetName", "");
    LOG(DEBUG)<< getProperty<string>("class", "");
    AbstractItem* ret = game::serialization::instantiateItemClass(getProperty<string>("class", ""));
    ret->properties = this->properties; // Clone properties
    LOG(DEBUG)<< ret->getProperty<string>("class", "");
    return ret;
}
