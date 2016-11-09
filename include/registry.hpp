#ifndef INCLUDE_REGISTRY_HPP_
#define INCLUDE_REGISTRY_HPP_
#include "level.hpp"
#include <string>

namespace game {

namespace tileregistry {
void init();
int nextId();
void bind(std::shared_ptr<game::level::AbstractTile> t);
std::weak_ptr<game::level::AbstractTile> byId(int id);
std::weak_ptr<game::level::AbstractTile> byName(const std::string& name);
}

}

#endif
