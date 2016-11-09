#ifndef INCLUDE_GAME_HPP_
#define INCLUDE_GAME_HPP_
#include "gfx.hpp"
#include "level.hpp"
#include "camera.hpp"

// Macros for easier game object access
#define G game::Game::instance()
#define GR game::Game::instance()->renderer()
#define GL game::Game::instance()->currentLevel()
#define GC game::Game::instance()->camera()

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

    // Returns renderer
    game::gfx::Renderer* renderer();

    // Returns current level
    std::weak_ptr<game::level::Level> currentLevel();

    std::weak_ptr<game::level::Camera> camera();

    // Event callback
    void sdlEvent(SDL_Event* e);
private:
    // Private constructor
    Game();
    // Internal start method
    void startInternal();

    // Instance variable
    static Game* mInstance;
    // Renderer
    game::gfx::Renderer* mRenderer;
    // Current level pointer
    std::shared_ptr<game::level::Level> mCurrentLevel;
    // Camera
    std::shared_ptr<game::level::Camera> mCamera;
    // Holds game state
    bool mRunning;
};

}

#endif
