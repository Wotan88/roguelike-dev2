#ifndef INCLUDE_ENTITIES_GOBLIN_HPP_
#define INCLUDE_ENTITIES_GOBLIN_HPP_
#include "entity.hpp"

namespace game {
namespace level {
namespace entity {

// Goblin entity
class GoblinEntity: public AbstractEntity {
public:
    GoblinEntity(Level* l);
    virtual ~GoblinEntity() {
    }
    void onAttackedBy(int dmg, AbstractEntity* src) override;
    void onTick(int tickN) override;
};
}
}
}

#endif
