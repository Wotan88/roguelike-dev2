#ifndef INCLUDE_INVENTORY_HPP_
#define INCLUDE_INVENTORY_HPP_
#include <string>
#include <vector>

namespace game {
namespace item {

class AbstractItem;

struct InventoryItem {
    AbstractItem* item;
    int count;
};

class Inventory {
public:
    Inventory(int sz);
    virtual ~Inventory();

    int findSlot(int id);
    bool addItem(const std::string& name, int count = 1);
    bool addItemI(AbstractItem* i, int count = 1);
    void setSlot(int i, int c, int v);
    void setItem(int i, AbstractItem* t, int v);
    int getSize();
    InventoryItem& getSlot(int i);
    void updateCount(int i, int delta);
    bool hasItemInSlot(int i);
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
    void setSlot(int i, AbstractItem* t, int v);
    bool addItemI(AbstractItem* i, int count = 1);
private:
    Inventory mInventory;
};

class EquipmentHolder {
public:
    EquipmentHolder(int sz);
    virtual ~EquipmentHolder();

    bool setEquipmentSlot(int s, const std::string& name);
    bool setEquipmentItem(int s, AbstractItem* i);
    void clearEquipmentSlot(int s);
    bool hasItemEquiped(int s);
    AbstractItem* getEquipedItem(int s);
    std::string getEquipmentSlotName(int s);
    int getEquipmentInventorySize();
    void setEquipmentSlotName(int s, const std::string& n);
private:
    Inventory mEquipmentInventory;
    std::vector<std::string> mSlotNames;
};

}
}

#endif
