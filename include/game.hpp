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

    void fullRender();

    void generatePlayerAndStart(const vector<std::pair<string, int>>& attrs);

    // Returns renderer
    game::gfx::Renderer* renderer();

    // Returns current level
    game::level::Level* currentLevel();

    game::level::Camera* camera();

    game::level::Player* player();

    game::gui::AbstractGUI* gui();

    int state();

    void updateCamera();

    void endTurn();

    // Event callback
    void sdlEvent(SDL_Event* e);

    void nextDepth();
    void prevDepth();

    static constexpr int STATE_PLAYING = 1;
private:
    // Private constructor
    Game();
    // Internal start method
    void startInternal();

    void initGame();
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

    game::gui::AbstractGUI* mCurrentGui;
    // Holds game state
    int mState;
    bool mRunning;
    int mCurrentDepth;
    int mTickNumber;
};

}

#endif
