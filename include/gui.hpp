#ifndef INCLUDE_GUI_HPP_
#define INCLUDE_GUI_HPP_
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace game {
namespace level {
class Player;
}

namespace gui {

class AbstractGUI {
public:
    AbstractGUI();
    virtual ~AbstractGUI();

    virtual bool capturesKeyboard();
    virtual bool onKeyDown(int sc, int ch);
    virtual void render();
};

class EmbarkGUI: public AbstractGUI {
public:
    EmbarkGUI();
    ~EmbarkGUI();

    bool capturesKeyboard() override;
    bool onKeyDown(int sc, int ch) override;
    void render() override;
private:
    vector<std::pair<string, int>> mAttributes;
    int mPointsLeft;
    int mSelectedItem;
};

class InventoryGUI: public AbstractGUI {
public:
    InventoryGUI();
    ~InventoryGUI();

    bool capturesKeyboard() override;
    bool onKeyDown(int sc, int ch) override;
    void render() override;
private:
    level::Player* mPlayer;
};

}
}

#endif
