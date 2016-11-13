#include "item.hpp"
#include "registry.hpp"

game::item::Inventory::Inventory(int sz) {
    mInternalArray = std::vector<InventoryItem>(sz);
    mSize = sz;
}

game::item::Inventory::~Inventory() {
}

int game::item::Inventory::findSlot(int id) {
    for (int i = 0; i < mSize; i++) {
        if (mInternalArray[i].itemId == id)
            return i;
    }
    return -1;
}

int game::item::Inventory::getSize() {
    return mSize;
}

void game::item::Inventory::setSlot(int i, int c, int v) {
    mInternalArray[i].itemId = c;
    mInternalArray[i].count = v;
}

bool game::item::Inventory::addItem(const string& name, int count) {
    game::item::AbstractItem* g = game::itemregistry::byName(name);

    if (!g)
        return false;

    int id = g->getProperty<int>("id", -1);
    if (id == -1)
        return false;

    LOG(DEBUG)<< "Trying to add " << count << "x" << name;

    int slot0 = findSlot(id);
    if (slot0 != -1) {
        mInternalArray[slot0].count += count;
        return true;
    }
    int slot1 = findSlot(0);
    if (slot1 != -1) {
        mInternalArray[slot1].itemId = id;
        mInternalArray[slot1].count = count;
        return true;
    }

    return false;
}

game::item::InventoryItem& game::item::Inventory::getSlot(int i) {
    return mInternalArray[i];
}

game::item::InventoryHolder::InventoryHolder(int sz) :
        mInventory(sz) {
}
game::item::InventoryHolder::~InventoryHolder() {
}

bool game::item::InventoryHolder::addItem(const string& name, int count) {
    return mInventory.addItem(name, count);
}

int game::item::InventoryHolder::getInventorySize() {
    return mInventory.getSize();
}

game::item::InventoryItem& game::item::InventoryHolder::getItem(int i) {
    return mInventory.getSlot(i);
}
