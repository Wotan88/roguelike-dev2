#include "items/food.hpp"

bool game::item::FoodItem::onUsed(level::AbstractEntity* src) {
    if (src) {
        int oldHL = src->getProperty<int>("hunger", -1);
        if (oldHL != -1) {
            oldHL -= getProperty<int>("nutrition", 0);
            src->setProperty<int>("hunger", oldHL);
        }
    }
    return true;
}
