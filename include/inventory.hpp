#ifndef INCLUDE_INVENTORY_HPP_
#define INCLUDE_INVENTORY_HPP_
#include <string>
#include <vector>

namespace game {
namespace item {

struct InventoryItem {
    int itemId, count;
};

class Inventory {
public:
    Inventory(int sz);
    virtual ~Inventory();

    int findSlot(int id);
    bool addItem(const std::string& name, int count = 1);
    void setSlot(int i, int c, int v);
    int getSize();
    InventoryItem& getSlot(int i);
    void updateCount(int i, int delta);
private:
    std::vector<InventoryItem> mInternalArray;
    int mSize;
};

class InventoryHolder {
public:
    InventoryHolder(int sz);
    virtual ~InventoryHolder();

    bool addItem(const std::string& name, int count = 1);
    int getInventorySize();
    InventoryItem& getItem(int i);
    void updateCount(int i, int delta);
private:
    Inventory mInventory;
};

class EquipmentHolder {
public:
    EquipmentHolder(int sz);
    virtual ~EquipmentHolder();

    bool setEquipmentSlot(int s, const std::string& name);
    void clearEquipmentSlot(int s);
    int getEquipmentSlotCount();
private:
    Inventory mEquipmentInventory;
};

}
}

#endif
