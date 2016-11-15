#ifndef INCLUDE_ITEMS_MELEEWEAPON_HPP_
#define INCLUDE_ITEMS_MELEEWEAPON_HPP_
#include "item.hpp"

namespace game {
namespace item {

class MeleeWeapon: public AbstractItem {
public:
    MeleeWeapon();
    virtual ~MeleeWeapon();

    bool isEquipable() override;
    bool canAttackWith() override;
    bool onAttackWith(level::AbstractEntity* src, level::AbstractEntity* target) override;
};

}
}

#endif
