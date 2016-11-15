#ifndef INCLUDE_ITEM_HPP_
#define INCLUDE_ITEM_HPP_
#include "structs.hpp"
#include "core.hpp"

namespace game {
namespace level {
class AbstractEntity;
}

namespace item {

class AbstractItem: public game::data::PropertyHolder {
public:
    AbstractItem();
    virtual ~AbstractItem();

    AbstractItem* clone();

    virtual bool onUsed(level::AbstractEntity* src);
    virtual bool isUsable();
    virtual bool isEquipable();
    virtual bool canAttackWith();
    virtual bool onAttackWith(level::AbstractEntity* src, level::AbstractEntity* target);
};

}
}

#endif
