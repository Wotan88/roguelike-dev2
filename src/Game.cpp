#include "game.hpp"
#include "registry.hpp"
#include "serialization.hpp"
#include "generators.hpp"
#include "general_utils.hpp"
#include "entities/goblin.hpp"
#include "messages.hpp"

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

    mSelectorX = 0;
    mSelectorY = 0;
    mControlMode = 0;

    mCurrentDepth = 0;
    // TODO: move to level
    mTickNumber = 0;
    mState = 0;
    mCurrentGui = nullptr;
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

void game::Game::endTurn() {
    int php = mPlayer->getProperty<int>("hp", -1);
    LOG(DEBUG)<< php;
    if (php <= 0) {
        LOG(DEBUG)<< "PLAYER DIED";
        mRunning = false;
        return;
    }

    for (auto e : mCurrentLevel->entities()) {
        if (e)
            e->onTick(mTickNumber);
    }
    mTickNumber++;

    updateCamera();
}

void game::Game::fullRender() {
    mRenderer->clear();
    if (mState == STATE_PLAYING) {
        mRenderer->renderLevel(0, 0, game::gfx::SCREEN_WIDTH - 25,
                game::gfx::SCREEN_HEIGHT);
        mRenderer->renderEntities();
        if (mControlMode == 1) {
            int sx, sy;
            mCamera->translatePoint(mSelectorX, mSelectorY, sx, sy);
            mRenderer->put(sx, sy, -1, 0, 0xDDDDDD);
            int px, py;
            mPlayer->getPosition(px, py);
            float angle = std::atan2(mSelectorY - py, mSelectorX - px);
            float dist = std::floor(
                    std::sqrt(
                            std::pow(mSelectorX - px, 2)
                                    + std::pow(mSelectorY - py, 2)));
            float fx, fy;
            float dx, dy;
            dx = std::cos(angle);
            dy = std::sin(angle);
            fx = px + 0.5;
            fy = py + 0.5;
            int ix, iy, isx, isy;
            ix = 0;
            iy = 0;
            for (int i = 0; i < dist; i++) {
                fx += dx;
                fy += dy;
                ix = (int) fx;
                iy = (int) fy;
                mCamera->translatePoint(ix, iy, isx, isy);
                mRenderer->put(isx, isy, -1, 0, 0xBBBBBB);
            }
        }
        mRenderer->renderHud();
    }
    if (mCurrentGui) {
        mRenderer->renderGui();
    }
    mRenderer->renderAll();
}

void game::Game::initGame() {

}

void game::Game::setGui(game::gui::AbstractGUI* g) {
    delete mCurrentGui;
    mCurrentGui = g;
}

void game::Game::generatePlayerAndStart(
        const vector<std::pair<string, int>>& attrs) {
    mCurrentLevel = new level::Level(200, 120);
    level::depths::push(mCurrentLevel);
    mCamera = new level::Camera(0, 0);
    mPlayer = new level::Player(mCurrentLevel);

    mPlayer->addItem("food:small", 2);
    genLevel();
    mCurrentLevel->update();
    int px, py;
    mPlayer->getPosition(px, py);

    for (auto it : attrs) {
        mPlayer->setAttribute(it.first, it.second);
    }

    mCamera->center(px, py);

    setGui(nullptr);
    mState = STATE_PLAYING;

    mRenderer->enableRenderTicking();
    fullRender();
}

void game::Game::startInternal() {
    mRenderer = new gfx::Renderer();
    mRenderer->loadResources();

    game::serialization::loadAllTiles("assets/tiles/");
    auto i = game::serialization::loadItem("assets/items/smallFood.json");
    game::itemregistry::bind(i);

    mCurrentGui = new game::gui::EmbarkGUI();

    fullRender();

    mRunning = true;
    while (mRunning) {
        mRenderer->pollEvents();
    }

    LOG(INFO)<< "Stopping";

    SDL_Quit();
}

int game::Game::state() {
    return mState;
}

void game::Game::updateCamera() {
    int px, py, cx, cy;

    if (mControlMode == 0) {
        mPlayer->getPosition(px, py);
    } else {
        px = mSelectorX;
        py = mSelectorY;
    }
    mCamera->translatePoint(px, py, cx, cy);

    while (cx <= 10) {
        mCamera->move(-1, 0);
        mCamera->translatePoint(px, py, cx, cy);
    }

    while (cy <= 9) {
        mCamera->move(0, -1);
        mCamera->translatePoint(px, py, cx, cy);
    }

    while (cx >= game::gfx::SCREEN_WIDTH - 25 - 10) {
        mCamera->move(1, 0);
        mCamera->translatePoint(px, py, cx, cy);
    }

    while (cy >= game::gfx::SCREEN_HEIGHT - 9 - 1) {
        mCamera->move(0, 1);
        mCamera->translatePoint(px, py, cx, cy);
    }

    fullRender();
}

void game::Game::moveControl(int dx, int dy) {
    int px, py;
    mPlayer->getPosition(px, py);
    if (mControlMode == 0) {
        if (mPlayer->checkMove(dx, dy)) {
            LOG(DEBUG)<< "Move caused player to end turn";
            mPlayer->move(dx, dy);
            mCurrentLevel->update();
            endTurn();
        } else {
            if (mPlayer->canAttack(px + dx, py + dy)) {
                LOG(DEBUG)<< "Attack caused player to end turn";
                // Attack entity
                level::AbstractEntity* e = mCurrentLevel->getEntityAt(px + dx, py + dy);
                if (e) {
                    messages::push("You hit " + e->getProperty<string>("name", ""));
                    e->onAttackedBy(1, mPlayer);
                }
                // End turn
                mCurrentLevel->update();
                endTurn();
            } else if (mPlayer->onCollideTile(px + dx, py + dy)) {
                LOG(DEBUG)<< "Collision caused player to end turn";
                mCurrentLevel->update();
                endTurn();
            }
        }
    } else {
        mSelectorX += dx;
        mSelectorY += dy;
        updateCamera();
    }
}

void game::Game::shootProjectile(int x, int y) {
    int px, py;
    mPlayer->getPosition(px, py);

    float angle = std::atan2(y - py, x - px);
    float dist = std::floor(
            std::sqrt(
                    std::pow(mSelectorX - px, 2)
                            + std::pow(mSelectorY - py, 2)));
    float fx, fy;
    float dx, dy;
    dx = std::cos(angle);
    dy = std::sin(angle);
    fx = px + 0.5;
    fy = py + 0.5;
    int ix, iy;

    for (int i = 0; i < dist; i++) {
        fx += dx;
        fy += dy;

        ix = (int) fx;
        iy = (int) fy;

        level::AbstractEntity* e = mCurrentLevel->getEntityAt(ix, iy);
        if (e) { // We hit an entity
            break;
        }

        level::AbstractTile* t = (*mCurrentLevel)(ix, iy);

        if (t && t->isCollidable(ix, iy, mCurrentLevel)) { // We hit collidable tile
            break;
        }

        mCamera->translatePoint(ix, iy, ix, iy);

        mRenderer->renderLevel(ix - 1, iy - 1, ix + 1, iy + 1);
        mRenderer->put(ix, iy, '*', 0xFFFFFF, 0);
        mRenderer->renderAll();

        SDL_Delay(50);
    }

    mControlMode = 0;
    fullRender();
}

void game::Game::gameControl(SDL_Keysym* k) {
    int scancode = k->scancode;
    int ch = k->sym;
    if (k->mod & KMOD_SHIFT) {
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

    if (scancode == SDL_SCANCODE_T) {
        if (mControlMode == 0) {
            mPlayer->getPosition(mSelectorX, mSelectorY);
            mControlMode = 1;
        } else {
            shootProjectile(mSelectorX, mSelectorY);
        }
        return;
    }
    if (scancode == SDL_SCANCODE_I) {
        if (mCurrentGui == nullptr) {
            game::gui::InventoryGUI* ui = new game::gui::InventoryGUI();
            setGui(ui);
            fullRender();
        }
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

void game::Game::sdlEvent(SDL_Event* e) {
    if (e->type == SDL_QUIT) {
        LOG(DEBUG)<< "Quit event received";

        mRunning = false;
        return;
    }
    if (e->type == SDL_KEYDOWN) {
        if (mState == STATE_PLAYING) {
            if (mCurrentGui) {
                if (mCurrentGui->capturesKeyboard()) {
                    mCurrentGui->onKeyDown(e->key.keysym.scancode, e->key.keysym.sym);
                    return;
                }
            }
            gameControl(&(e->key.keysym));
        } else {
            if (mCurrentGui) {
                mCurrentGui->onKeyDown(e->key.keysym.scancode, e->key.keysym.sym);
            }
        }
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

game::gui::AbstractGUI* game::Game::gui() {
    return mCurrentGui;
}
