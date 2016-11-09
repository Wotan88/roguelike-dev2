#ifndef INCLUDE_SERIALIZATION_HPP_
#define INCLUDE_SERIALIZATION_HPP_
#include "level.hpp"

#include <string>
using std::string;

namespace game {
namespace serialization {

std::shared_ptr<game::level::AbstractTile> instantiateTileClass(const string& basename);
std::shared_ptr<game::level::AbstractTile> loadTile(const string& filename);
void loadAllTiles(const string& dir);

}
}

#endif
