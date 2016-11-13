#include "items/food.hpp"
#include "messages.hpp"

game::item::FoodItem::FoodItem() {
    setProperty<bool>("usable", true);
}

bool game::item::FoodItem::onUsed(level::AbstractEntity* src) {
    if (src) {
        if (src->getProperty<string>("class", "") == "Player") {
            game::messages::push(
                    "You eat " + getProperty<string>("name", "food") + ". "
                            + getProperty<string>("eatenMessage", ""));
        }
        int oldHL = src->getProperty<int>("hunger", -1);
        if (oldHL != -1) {
            oldHL -= getProperty<int>("nutrition", 0);
            src->setProperty<int>("hunger", oldHL);
        }
    }
    return true;
}
