#ifndef INCLUDE_ENTITY_HPP_
#define INCLUDE_ENTITY_HPP_
#include "structs.hpp"
#include "level.hpp"
#include <string>
using std::string;

namespace game {
namespace level {

class AbstractEntity: public game::data::PropertyHolder<string> {
public:
    // Constructor
    AbstractEntity(game::level::Level* l);
    // Destructor
    virtual ~AbstractEntity();

private:
    // Level pointer
    game::level::Level* mLevel;
};

}
}

#endif
