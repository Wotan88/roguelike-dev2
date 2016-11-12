#include "game.hpp"
#include "registry.hpp"
#include "serialization.hpp"
#include "generators.hpp"
#include "general_utils.hpp"

#include <easylogging++.h>

using game::Game;

// Game::mInstance
Game* game::Game::mInstance = nullptr;

game::Game::Game() {
    mRenderer = nullptr;
    mRunning = false;
    mCurrentLevel = nullptr;
    mCamera = nullptr;
    mPlayer = nullptr;

    mCurrentDepth = 0;
}

game::Game::~Game() {
    LOG(DEBUG)<< "Game object deallocated";

    if (mRenderer) {
        delete mRenderer;
    }
    if (mCurrentLevel) {
        delete mCurrentLevel;
    }
    if (mCamera) {
        delete mCamera;
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

void game::Game::genLevel() {
    game::level::gen::DungeonGenerator g;
    g.setProperty<string>("downstairs", "stairs:down");
    g.setProperty<string>("floor", "floor:stone");
    g.setProperty<string>("wall", "wall:stone");
    g.setProperty<string>("door", "door:wood");
    g.setProperty<string>("upstairs", "stairs:up");
    g.generate(mCurrentLevel);
    int psx, psy;
    g.getSpawnPosition(psx, psy);
    mPlayer->setPosition(psx, psy);
    mCurrentLevel->addEntity(mPlayer);
}

void game::Game::nextDepth() {
    level::Level* l = game::level::depths::get(mCurrentDepth + 1);
    int x, y;
    mPlayer->getPosition(x, y);
    if (l) {
        mCurrentLevel->onPlayerDown(x, y);
        mCurrentDepth++;

        int px, py;
        l->getUpExitPos(px, py);
        mPlayer->setPosition(px, py);
        l->addEntity(mPlayer);
        mCurrentLevel = l;

        mCamera->center(px, py);
        mCurrentLevel->update();
    } else {
        mCurrentLevel->onPlayerDown(x, y);
        mCurrentDepth++;
        mCurrentLevel = new level::Level(200, 120);

        genLevel();

        int px, py;
        mPlayer->getPosition(px, py);
        mCamera->center(px, py);

        level::depths::push(mCurrentLevel);
        mCurrentLevel->update();
    }
    fullRender();
}

void game::Game::prevDepth() {
    int x, y;
    mPlayer->getPosition(x, y);
    if (mCurrentDepth != 0) {
        mCurrentLevel->onPlayerUp(x, y);

        level::Level* l = game::level::depths::get(mCurrentDepth - 1);
        int px, py;
        l->getDownExitPos(px, py);
        mPlayer->setPosition(px, py);
        mCurrentLevel = l;
        mCurrentLevel->addEntity(mPlayer);
        mCurrentDepth--;
        mCamera->center(px, py);
        mCurrentLevel->update();

        fullRender();
    }
}

void game::Game::fullRender() {
    mRenderer->clear();
    mRenderer->renderLevel(0, 0, game::gfx::SCREEN_WIDTH - 25,
            game::gfx::SCREEN_HEIGHT);
    mRenderer->renderEntities();
    mRenderer->renderAll();
}

void game::Game::startInternal() {
    mRenderer = new gfx::Renderer();
    mRenderer->loadResources();

    mCurrentLevel = new level::Level(200, 120);
    level::depths::push(mCurrentLevel);

    mCamera = new level::Camera(0, 0);

    mPlayer = new level::Player(mCurrentLevel);

    game::serialization::loadAllTiles("assets/tiles/");

    genLevel();

    mCurrentLevel->update();
    int px, py;
    mPlayer->getPosition(px, py);
    mCamera->center(px, py);

    fullRender();

    mRunning = true;
    while (mRunning) {
        mRenderer->pollEvents();
    }

    LOG(INFO)<< "Stopping";

    SDL_Quit();
}

int game::Game::state() {
    return STATE_PLAYING;
}

void game::Game::updateCamera() {
    int px, py, cx, cy;
    mPlayer->getPosition(px, py);
    mCamera->translatePoint(px, py, cx, cy);

    while (cx <= 5) {
        mCamera->move(-1, 0);
        mCamera->translatePoint(px, py, cx, cy);
    }

    while (cy <= 4) {
        mCamera->move(0, -1);
        mCamera->translatePoint(px, py, cx, cy);
    }

    while (cx >= game::gfx::SCREEN_WIDTH - 25 - 5) {
        mCamera->move(1, 0);
        mCamera->translatePoint(px, py, cx, cy);
    }

    while (cy >= game::gfx::SCREEN_HEIGHT - 4 - 1) {
        mCamera->move(0, 1);
        mCamera->translatePoint(px, py, cx, cy);
    }

    fullRender();
}

void game::Game::moveControl(int dx, int dy) {
    int px, py;
    mPlayer->getPosition(px, py);
    if (mPlayer->checkMove(dx, dy)) {
        mPlayer->move(dx, dy);
        mCurrentLevel->update();
        updateCamera();
    } else {
        if (mPlayer->onCollideTile(px + dx, py + dy)) {
            mCurrentLevel->update();
            updateCamera();
        }
    }
}

void game::Game::sdlEvent(SDL_Event* e) {
    if (e->type == SDL_QUIT) {
        LOG(DEBUG)<< "Quit event received";

        mRunning = false;
        return;
    }
    if (e->type == SDL_KEYDOWN) {
        int scancode = e->key.keysym.scancode;
        int ch = e->key.keysym.sym;
        if (e->key.keysym.mod & KMOD_SHIFT) {
            ch = game::util::shiftKey(ch);
        }

        LOG(DEBUG)<<"Keydown: "<<scancode;

        int px, py;
        mPlayer->getPosition(px, py);

        // TODO: input handler
        if (ch == SDLK_KP_0) {
            mRenderer->nextRenderMode();
            fullRender();
            return;
        }
        if (ch == SDLK_GREATER) {
            LOG(DEBUG)<< "Downstairs key";
            level::AbstractTile* t = (*mCurrentLevel)(px, py);

            if (t && t->isDownstairs(px, py, mCurrentLevel)) {
                LOG(DEBUG)<< "Next depth";
                nextDepth();
            }
            return;
        }
        if (ch == SDLK_LESS) {
            level::AbstractTile* t = (*mCurrentLevel)(px, py);

            if (t && t->isUpstairs(px, py, mCurrentLevel)) {
                LOG(DEBUG)<< "Prev depth";
                prevDepth();
            }
            return;
        }

        switch (ch) {
            case SDLK_KP_1:
            moveControl(-1, 1);
            return;
            case SDLK_KP_2:
            moveControl(0, 1);
            return;
            case SDLK_KP_3:
            moveControl(1, 1);
            return;
            case SDLK_KP_4:
            moveControl(-1, 0);
            return;
            case SDLK_KP_6:
            moveControl(1, 0);
            return;
            case SDLK_KP_7:
            moveControl(-1, -1);
            return;
            case SDLK_KP_8:
            moveControl(0, -1);
            return;
            case SDLK_KP_9:
            moveControl(1, -1);
            return;
        }

        return;
    }
}

game::gfx::Renderer* game::Game::renderer() {
    return mRenderer;
}

game::level::Level* game::Game::currentLevel() {
    return mCurrentLevel;
}

game::level::Camera* game::Game::camera() {
    return mCamera;
}

game::level::Player* game::Game::player() {
    return mPlayer;
}
