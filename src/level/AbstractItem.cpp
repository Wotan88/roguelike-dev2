#include "item.hpp"

game::item::AbstractItem::AbstractItem() {
}

game::item::AbstractItem::~AbstractItem() {
}

bool game::item::AbstractItem::onUsed(level::AbstractEntity* src) {
    return false;
}

bool game::item::AbstractItem::isEquipable(){
    return getProperty<bool>("equipable", false);
}

bool game::item::AbstractItem::isUsable(){
    return getProperty<bool>("usable", false);
}
