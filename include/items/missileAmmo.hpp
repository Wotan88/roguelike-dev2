#ifndef INCLUDE_ITEMS_MISSILEAMMO_HPP_
#define INCLUDE_ITEMS_MISSILEAMMO_HPP_
#include "item.hpp"

namespace game {
namespace item {

class MissileAmmo: public AbstractItem {
public:
    MissileAmmo();
    virtual ~MissileAmmo();

    bool isEquipable() override;
    bool onHitEntity(level::AbstractEntity* src, level::AbstractEntity* target, int weaponBonus);
};

}
}

#endif
