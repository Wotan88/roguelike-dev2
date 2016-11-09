#include "game.hpp"
#include "registry.hpp"
#include "serialization.hpp"
#include "generators.hpp"

#include <easylogging++.h>

using game::Game;

// Game::mInstance
Game* game::Game::mInstance = nullptr;

game::Game::Game() {
    mRenderer = nullptr;
    mRunning = false;
    mCurrentLevel = nullptr;
    mCamera = nullptr;
}

game::Game::~Game() {
    LOG(DEBUG)<< "Game object deallocated";

    if (mRenderer) {
        delete mRenderer;
    }
}

Game* game::Game::instance() {
    return mInstance;
}

void game::Game::start() {
    // Create new instance of game
    if (mInstance) {
        LOG(FATAL)<< "Game instance already created";
    }
    mInstance = new Game();

    // Call private start method
    mInstance->startInternal();

    // Deallocate game instance
    delete mInstance;
}

void game::Game::startInternal() {
    mRenderer = new gfx::Renderer();
    mRenderer->loadResources();

    mCurrentLevel = std::make_shared<game::level::Level>(
            level::Level(200, 120));

    mCamera = std::make_shared<game::level::Camera>(level::Camera(0, 0));

    game::serialization::loadAllTiles("assets/tiles/");

    {
        game::level::gen::DungeonGenerator g;
        g.setProperty<string>("floor", "floor:stone");
        g.setProperty<string>("wall", "wall:stone");
        g.setProperty<string>("door", "door:wood");
        g.generate(mCurrentLevel);
    }

    mRunning = true;

    mRenderer->renderLevel(0, 0, game::gfx::SCREEN_WIDTH,
            game::gfx::SCREEN_HEIGHT);
    mRenderer->renderAll();

    while (mRunning) {
        mRenderer->pollEvents();
    }

    LOG(INFO)<< "Stopping";

    SDL_Quit();
}

void game::Game::sdlEvent(SDL_Event* e) {
    if (e->type == SDL_QUIT) {
        LOG(DEBUG)<< "Quit event received";

        mRunning = false;
        return;
    }
    if (e->type == SDL_KEYDOWN) {
        int scancode = e->key.keysym.scancode;
        LOG(DEBUG)<<"Keydown: "<<scancode;

        // TODO: input handler

        if (scancode == 92) {
            mCamera->move(-1, 0);
            mRenderer->clear();
            mRenderer->renderLevel(0, 0, game::gfx::SCREEN_WIDTH,
                    game::gfx::SCREEN_HEIGHT);
            mRenderer->renderAll();
            return;
        }
//
        if (scancode == 94) {
            mCamera->move(1, 0);
            mRenderer->clear();
            mRenderer->renderLevel(0, 0, game::gfx::SCREEN_WIDTH,
                    game::gfx::SCREEN_HEIGHT);
            mRenderer->renderAll();
            return;
        }
//
        if (scancode == 90) {
            mCamera->move(0, 1);
            mRenderer->clear();
            mRenderer->renderLevel(0, 0, game::gfx::SCREEN_WIDTH,
                    game::gfx::SCREEN_HEIGHT);
            mRenderer->renderAll();
            return;
        }

        if (scancode == 96) {
            mCamera->move(0, -1);
            mRenderer->clear();
            mRenderer->renderLevel(0, 0, game::gfx::SCREEN_WIDTH,
                    game::gfx::SCREEN_HEIGHT);
            mRenderer->renderAll();
            return;
        }
//
//        if (scancode == SDL_KP_2) {
//            mCamera->move(0, 1);
//            mRenderer->renderLevel(0, 0, game::gfx::SCREEN_WIDTH,
//                    game::gfx::SCREEN_HEIGHT);
//            mRenderer->renderAll();
//            return;
//        }

        return;
    }
}

game::gfx::Renderer* game::Game::renderer() {
    return mRenderer;
}

std::weak_ptr<game::level::Level> game::Game::currentLevel() {
    return mCurrentLevel;
}

std::weak_ptr<game::level::Camera> game::Game::camera() {
    return mCamera;
}
