#include "item.hpp"
#include "registry.hpp"

game::item::Inventory::Inventory(int sz) {
    mInternalArray = std::vector<InventoryItem>(sz);
    mSize = sz;
}

game::item::Inventory::~Inventory() {
}

int game::item::Inventory::findSlot(int id) {
    if (id <= 0) {
        for (int i = 0; i < mSize; i++) {
            if (mInternalArray[i].item == nullptr)
                return i;
        }
    } else {
        for (int i = 0; i < mSize; i++) {
            if (mInternalArray[i].item
                    && mInternalArray[i].item->getProperty<int>("id", -1) == id)
                return i;
        }
    }
    return -1;
}

bool game::item::Inventory::hasItemInSlot(int i) {
    return (mInternalArray[i].item != nullptr);
}

int game::item::Inventory::getSize() {
    return mSize;
}

void game::item::Inventory::setSlot(int i, int c, int v) {
    if (v <= 0) {
        mInternalArray[i].item = nullptr;
        mInternalArray[i].count = 0;
    } else {
        AbstractItem* t = game::itemregistry::byId(c);
        if (t) {
            mInternalArray[i].item = t->clone();
            mInternalArray[i].count = v;
        } else {
            mInternalArray[i].item = nullptr;
            mInternalArray[i].count = 0;
        }
    }
}

void game::item::Inventory::setItem(int i, AbstractItem* t, int v) {
    if (t) {
        mInternalArray[i].item = t;
        mInternalArray[i].count = v;
    } else {
        mInternalArray[i].item = nullptr;
        mInternalArray[i].count = 0;
    }
}

void game::item::Inventory::updateCount(int i, int delta) {
    if (mInternalArray[i].count > 0 && mInternalArray[i].item) {
        int newcount = std::max(0, mInternalArray[i].count + delta);
        if (newcount == 0) {
            mInternalArray[i].item = nullptr;
        }
        mInternalArray[i].count = newcount;
    }
}

bool game::item::Inventory::addItem(const string& name, int count) {
    game::item::AbstractItem* g = game::itemregistry::byName(name);

    if (!g)
        return false;

    LOG(DEBUG)<< "Trying to add " << count << "x" << name;

    for (int i = 0; i < count; i++) {
        if (g->getProperty<bool>("stackable", false)) {
            int slot0 = findSlot(g->getProperty<int>("id", -1));
            if (slot0 != -1) {
                mInternalArray[slot0].count += 1;
                continue;
            }
        }
        int slot1 = findSlot(0);
        g = g->clone();
        if (slot1 != -1) {
            mInternalArray[slot1].item = g;
            mInternalArray[slot1].count = 1;
            continue;
        }
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

void game::item::InventoryHolder::updateCount(int i, int delta) {
    mInventory.updateCount(i, delta);
}

void game::item::InventoryHolder::setSlot(int i, AbstractItem* t, int v){
    if (!t || v <= 0){
        mInventory.setItem(i, nullptr, 0);
    } else {
        t = t->clone();
        mInventory.setItem(i, t, v);
    }
}
