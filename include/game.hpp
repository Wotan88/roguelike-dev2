#ifndef INCLUDE_GAME_HPP_
#define INCLUDE_GAME_HPP_
#include "gfx.hpp"
#include "level.hpp"
#include "camera.hpp"
#include "gui.hpp"

#include <vector>
using std::vector;

// Macros for easier game object access
#define G game::Game::instance()
#define GR game::Game::instance()->renderer()
#define GL game::Game::instance()->currentLevel()
#define GC game::Game::instance()->camera()
#define GP game::Game::instance()->player()

namespace game {

// Main game class
class Game {
public:
    // Destructor
    ~Game();
    // Instance getter
    static Game* instance();
    // Start method
    static void start();

    // Renders all objects
    void fullRender();

    // Creates player using attribute set (attrs) and starts game
    void generatePlayerAndStart(const vector<std::pair<string, int>>& attrs);

    // Returns game state (playing, paused, etc.)
    int state();

    // Changes current level
    void nextDepth();
    void prevDepth();

    // Updates camera position
    void updateCamera();

    // Shoots projectile (TODO: implement item binding)
    void shootProjectile(int x, int y);

    // Sets GUI (and calls destructor on previous, if present)
    void setGui(game::gui::AbstractGUI* g);

    // Returns renderer
    game::gfx::Renderer* renderer();
    // Returns current level
    game::level::Level* currentLevel();
    // Returns camera
    game::level::Camera* camera();
    // Returns player
    game::level::Player* player();
    // Returns current GUI
    game::gui::AbstractGUI* gui();

    // Event callback
    void sdlEvent(SDL_Event* e);
    // End current turn (pass control from player to AI)
    void endTurn();

    // Game states
    static constexpr int STATE_PLAYING = 1;
private:
    // Private constructor
    Game();
    // Internal start method
    void startInternal();

    void genLevel();

    void moveControl(int dx, int dy);
    void gameControl(SDL_Keysym* k);

    // Instance variable
    static Game* mInstance;
    // Renderer
    game::gfx::Renderer* mRenderer;
    // Current level pointer
    game::level::Level* mCurrentLevel;
    // Camera
    game::level::Camera* mCamera;
    // Player
    game::level::Player* mPlayer;

    // GUI
    game::gui::AbstractGUI* mCurrentGui;
    game::gui::AbstractGUI* mDeleteGui;

    // Holds game state
    int mState;
    bool mRunning;
    int mCurrentDepth;
    // Number of current turn
    int mTickNumber;
    // "Cursor" position
    int mSelectorX, mSelectorY;
    int mControlMode;
};

}

#endif
