#include "item.hpp"

game::item::AbstractItem::AbstractItem() {
}

game::item::AbstractItem::~AbstractItem() {
}

bool game::item::AbstractItem::onUsed(level::AbstractEntity* src) {
    return false;
}
