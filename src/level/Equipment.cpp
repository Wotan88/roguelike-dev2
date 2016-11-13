#include "inventory.hpp"
#include "item.hpp"
#include "registry.hpp"

game::item::EquipmentHolder::EquipmentHolder(int sz) :
        mEquipmentInventory(sz) {
}
game::item::EquipmentHolder::~EquipmentHolder() {
}

bool game::item::EquipmentHolder::setEquipmentSlot(int i,
        const std::string& name) {
    game::item::AbstractItem* g = game::itemregistry::byName(name);
    if (!g)
        return false;

    int id = g->getProperty<int>("id", -1);
    if (id == -1)
        return false;

    mEquipmentInventory.setSlot(i, id, 1);
    return true;
}

void game::item::EquipmentHolder::clearEquipmentSlot(int s) {
    mEquipmentInventory.setSlot(s, 0, 0);
}
