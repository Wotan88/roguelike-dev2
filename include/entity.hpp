#ifndef INCLUDE_ENTITY_HPP_
#define INCLUDE_ENTITY_HPP_
#include "core.hpp"
#include "structs.hpp"

#include <string>
using std::string;

namespace game {
namespace level {

class Level;

// Abstract entity class, inherited by all entities
class AbstractEntity: public game::data::PropertyHolder {
public:
    // Constructor
    AbstractEntity(game::level::Level* l);
    // Clone method
    AbstractEntity* clone();
    // Destructor
    virtual ~AbstractEntity();

    // Level pointer
    void setLevel(Level* l);

    // Visual attribute getters
    virtual int getForegroundColor();
    virtual int getIconIndex();

    // Attributes
    void setAttribute(const string& name, int val);
    void updateAttribute(const string& name, int delta);
    int getAttribute(const string& name);

    // Interaction attributes
    virtual bool canAttack(int dx, int dy);
    bool checkMove(int dx, int dy);

    // Position
    int x();
    int y();
    void getPosition(int& x, int& y);
    void setPosition(int x, int y);
    void move(int dx, int dy);

    // Events
    virtual void onAttackedBy(int dmg, AbstractEntity* src);
    virtual void onTick(int tickN);
    virtual void onAttributesChanged();
    virtual bool onCollideTile(int x, int y);
private:
    // Position
    int mX, mY;
protected:
    // Level pointer
    game::level::Level* mLevel;
};

// Player entity
class Player: public game::level::AbstractEntity, public game::item::InventoryHolder, public game::item::EquipmentHolder {
public:
    // Constructor
    Player(Level* l);
    // Destructor
    ~Player();

    // Events
    bool onCollideTile(int x, int y) override;
    void onAttackedBy(int dmg, AbstractEntity* src) override;
private:
};

}
}

#endif
