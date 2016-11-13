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

    virtual bool onUsed(level::AbstractEntity* src);
};

}
}

#endif
