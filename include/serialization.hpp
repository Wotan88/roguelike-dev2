#ifndef INCLUDE_SERIALIZATION_HPP_
#define INCLUDE_SERIALIZATION_HPP_
#include "level.hpp"
#include "item.hpp"

#include <string>
using std::string;

namespace game {
namespace serialization {

game::level::AbstractTile* instantiateTileClass(const string& basename);
game::level::AbstractTile* loadTile(const string& filename);
void loadAllTiles(const string& dir);

game::level::AbstractEntity* instantiateEntityClass(const string& basename);

game::item::AbstractItem* instantiateItemClass(const string& basename);
game::item::AbstractItem* loadItem(const string& filename);

}
}

#endif
