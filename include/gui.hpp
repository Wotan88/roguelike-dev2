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

    void checkItem(int i);
    void performAction(int i, int name);

    bool capturesKeyboard() override;
    bool onKeyDown(int sc, int ch) override;
    void render() override;

    static constexpr int ACTION_USE = 1;
    static constexpr int ACTION_DROP = 2;
    static constexpr int ACTION_EQUIP = 2;
private:
    level::Player* mPlayer;
    int mCheckedItem;
    vector<std::tuple<char, int, string>> mActions;
};

}
}

#endif
