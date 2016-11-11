#ifndef INCLUDE_REGISTRY_HPP_
#define INCLUDE_REGISTRY_HPP_
#include "level.hpp"
#include <string>

namespace game {

namespace tileregistry {
void init();
int nextId();
void bind(game::level::AbstractTile* t);
game::level::AbstractTile* byId(int id);
game::level::AbstractTile* byName(const std::string& name);
}

}

#endif
