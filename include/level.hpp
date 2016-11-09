#ifndef INCLUDE_LEVEL_HPP_
#define INCLUDE_LEVEL_HPP_
#include "structs.hpp"
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace game {
namespace level {

class Level;

// Abstract Tile class
class AbstractTile: public game::data::PropertyHolder {
public:
    // Constructor
    AbstractTile();
    // Destructor
    virtual ~AbstractTile();

    // Getters
    int getId();
    bool isCollidable(int x, int y, std::shared_ptr<Level> l);

    // Setters
    void setId(int newId);

    virtual int getBackgroundColor(int x, int y, std::shared_ptr<Level>& l);
    virtual int getForegroundColor(int x, int y, std::shared_ptr<Level>& l);
    virtual int getIconIndex(int x, int y, std::shared_ptr<Level>& l);
private:
    int mId;
};

// Level class
class Level: public game::data::PropertyHolder {
public:
    // Constructor
    Level(int width, int height);
    // Destructor
    ~Level();

    // Getters
    int getWidth();
    int getHeight();

    // Returns struct with information about tile
    std::weak_ptr<AbstractTile> operator()(int x, int y);

    // Sets tile
    void set(int x, int y, const string& name);
private:
    // Size
    int mWidth, mHeight;
    // Tile IDs
    vector<int> mData;
};

// Abstract generator interface
class AbstractGenerator: public game::data::PropertyHolder {
public:
    // Constructor
    AbstractGenerator();
    // Destructor
    virtual ~AbstractGenerator();

    // Main function
    virtual void generate(std::shared_ptr<game::level::Level>& l);
};

}
}

#endif
