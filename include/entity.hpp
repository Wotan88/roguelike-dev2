#ifndef INCLUDE_ENTITY_HPP_
#define INCLUDE_ENTITY_HPP_
#include "structs.hpp"
#include "level.hpp"

#include <string>
using std::string;

namespace game {
namespace level {

class Level;

class AbstractEntity: public game::data::PropertyHolder {
public:
    // Constructor
    AbstractEntity(game::level::Level* l);
    // Destructor
    virtual ~AbstractEntity();

    virtual int getForegroundColor();
    virtual int getIconIndex();

    int x();
    int y();
    void getPosition(int& x, int& y);
    void setPosition(int x, int y);
    void move(int dx, int dy);

    bool checkMove(int dx, int dy);
    virtual bool onCollideTile(int x, int y);

    void setLevel(Level* l);
private:
    // Level pointer

    int mX, mY;
protected:
    game::level::Level* mLevel;
};

class Player: public game::level::AbstractEntity {
public:
    // Constructor
    Player(Level* l);
    // Destructor
    ~Player();

    bool onCollideTile(int x, int y) override;
private:
};

}
}

#endif
