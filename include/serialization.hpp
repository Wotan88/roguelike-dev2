#ifndef INCLUDE_SERIALIZATION_HPP_
#define INCLUDE_SERIALIZATION_HPP_
#include "level.hpp"
#include "item.hpp"

#include <string>
using std::string;

namespace game {
namespace serialization {

constexpr int LEVEL_FORMAT_SIGNATURE = 0xD21CA78C;

game::level::AbstractTile* instantiateTileClass(const string& basename);
game::level::AbstractTile* loadTile(const string& filename);
void loadAllTiles(const string& dir);

game::level::AbstractEntity* instantiateEntityClass(const string& basename);
game::level::AbstractEntity* loadEntity(const string& filename);
void loadAllEntities(const string& dir);

game::item::AbstractItem* instantiateItemClass(const string& basename);
game::item::AbstractItem* loadItem(const string& filename);
void loadAllItems(const string& dir);

void saveLevel(level::Level* level, const string& filename);
// TODO: to be implemented
level::Level* loadLevel(const string& filename);

}
}

#endif
