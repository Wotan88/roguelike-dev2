#ifndef INCLUDE_ITEMS_FOOD_HPP_
#define INCLUDE_ITEMS_FOOD_HPP_
#include "item.hpp"

namespace game {
namespace item {

// Generic food
class FoodItem: public AbstractItem {
public:
    FoodItem();

    bool onUsed(level::AbstractEntity* src) override;
};

}
}

#endif
