#ifndef INCLUDE_REGISTRY_HPP_
#define INCLUDE_REGISTRY_HPP_
#include "level.hpp"
#include "item.hpp"
#include <string>

namespace game {

namespace tileregistry {
void init();
int nextId();
void bind(game::level::AbstractTile* t);
game::level::AbstractTile* byId(int id);
game::level::AbstractTile* byName(const std::string& name);
std::map<string, int>& getMappingTable();
}

namespace entityregistry {
void init();
int nextId();
void bind(game::level::AbstractEntity* e);
game::level::AbstractEntity* byId(int id);
game::level::AbstractEntity* byName(const std::string& name);
}

namespace itemregistry {
void init();
int nextId();
void bind(game::item::AbstractItem* e);
game::item::AbstractItem* byId(int id);
game::item::AbstractItem* byName(const std::string& name);
}


}

#endif
