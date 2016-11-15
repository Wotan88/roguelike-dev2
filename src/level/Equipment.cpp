#include "inventory.hpp"
#include "item.hpp"
#include "registry.hpp"

game::item::EquipmentHolder::EquipmentHolder(int sz) :
        mEquipmentInventory(sz) {
    mSlotNames = std::vector<std::string>(sz);
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

bool game::item::EquipmentHolder::hasItemEquiped(int s) {
    return mEquipmentInventory.hasItemInSlot(s);
}

game::item::AbstractItem* game::item::EquipmentHolder::getEquipedItem(int s) {
    return mEquipmentInventory.getSlot(s).item;
}

bool game::item::EquipmentHolder::setEquipmentItem(int s, AbstractItem* i){
    mEquipmentInventory.setItem(s, i, 1);
}

int game::item::EquipmentHolder::getEquipmentInventorySize(){
    return mEquipmentInventory.getSize();
}

void game::item::EquipmentHolder::setEquipmentSlotName(int s, const std::string& n){
    mSlotNames[s] = n;
}

std::string game::item::EquipmentHolder::getEquipmentSlotName(int s){
    return mSlotNames[s];
}
