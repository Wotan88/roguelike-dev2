#include "gfx.hpp"
#include "game.hpp"

#include <SDL2/SDL_image.h>
#include <easylogging++.h>

game::gfx::Renderer::Renderer() :
        mBuffer(SCREEN_WIDTH, SCREEN_HEIGHT) {
    mTilesetTexture = nullptr;
    mLastTick = 0;
    mLastColor = -1;

    LOG(INFO)<< "Initializing SDL";
    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        LOG(ERROR)<< "SDL_Init failed";
        LOG(FATAL) << SDL_GetError();
    }

    LOG(INFO)<< "Creating SDL window";
    // Create SDL window object
    mWindow = SDL_CreateWindow("Window title", 100, 100, SCREEN_WIDTH_PIXELS,
            SCREEN_HEIGHT_PIXELS, SDL_WINDOW_SHOWN);

    if (!mWindow) {
        LOG(ERROR)<< "SDL_CreateWindow failed";
        LOG(FATAL) << SDL_GetError();
    }

    LOG(INFO)<< "Creating SDL renderer";
    // Create SDL renderer object
    mRenderer = SDL_CreateRenderer(this->mWindow, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        LOG(ERROR)<< "SDL_CreateRenderer failed";
        LOG(FATAL) << SDL_GetError();
    }

    LOG(INFO)<< "Initializing SDL_image";
    IMG_Init(IMG_INIT_PNG);
}

game::gfx::Renderer::~Renderer() {
    LOG(DEBUG)<< "Renderer object deallocated";

    // Free tileset texture
    if (mTilesetTexture) {
        delete mTilesetTexture;
    }
}

SDL_Renderer* game::gfx::Renderer::sdlRenderer() {
    return mRenderer;
}

void game::gfx::Renderer::renderAll() {
    TIMED_FUNC(timerObj);
    // Clear screen
    SDL_SetRenderDrawColor(this->mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(this->mRenderer);

    // Render logic here
    // Draw buffer
    CharacterWrapper c;
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            c = mBuffer(x, y);
            renderBufferItem(x, y, c);
        }
    }

    // Display everything
    SDL_RenderPresent(mRenderer);

    mLastColor = -1;
}

void game::gfx::Renderer::pollEvents() {
    while (SDL_PollEvent(&mEvent)) {
        G->sdlEvent(&mEvent);
    }

    long time = SDL_GetTicks();
    if (time - mLastTick >= 250) {
        mLastTick = time;

        renderAll();
    }
}

void game::gfx::Renderer::renderBufferItem(int x, int y,
        const CharacterWrapper& c) {
    if (!c.characterIndex)
        return;

    if (mLastColor != c.foregroundColor) {
        mLastColor = c.foregroundColor;
        mTilesetTexture->applyColor(mLastColor);
    }

    int bg = c.backgroundColor;
    SDL_Rect rect { x * CHAR_WIDTH, y * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT };

    SDL_SetRenderDrawColor(mRenderer, (bg >> 16) & 0xFF, (bg >> 8) & 0xFF,
            bg & 0xFF, 0xFF);
    SDL_RenderFillRect(mRenderer, &rect);

    mTilesetTexture->render(c.characterIndex, &rect);
}

void game::gfx::Renderer::put(int x, int y, int c, int fg, int bg) {
    mBuffer(x, y) = CharacterWrapper { c, bg, fg };
}

void game::gfx::Renderer::fill(int sx, int sy, int dx, int dy, int c, int fg,
        int bg) {
    CharacterWrapper ch { c, bg, fg };
    for (int x = sx; x < dx; x++) {
        for (int y = sy; y < dy; y++) {
            mBuffer(x, y) = ch;
        }
    }
}

void game::gfx::Renderer::loadResources() {
    mBuffer.clear();

    LOG(INFO)<< "Loading textures";
    mTilesetTexture = new SpriteSheet("assets/font.png", 16, 16);

    LOG(INFO)<< "Done";
}

void game::gfx::Renderer::clear() {
    mBuffer.clear();
}

void game::gfx::Renderer::renderLevel(int sx, int sy, int dx, int dy) {
    TIMED_FUNC(timerObj);
    std::weak_ptr<game::level::Camera> c = GC;
    std::weak_ptr<game::level::Level> l = GL;
    if (auto level = l.lock()) {
        if (auto cam = c.lock()) {
            std::weak_ptr<game::level::AbstractTile> t;
            int tx, ty;

            int cx, cy;
            cam->getPosition(cx, cy);
            int ex, ey;
            ex = cx + SCREEN_WIDTH - 25;
            ey = cy + SCREEN_HEIGHT - 2;

            // Level bounds constraint
            cx = std::max(cx, 0);
            cy = std::max(cy, 0);
            ex = std::min(ex, level->getWidth() - 1);
            ey = std::min(ey, level->getHeight() - 1);

            LOG(INFO)<< cx << ','<<cy<<','<<ex<<','<<ey;

            for (int y = cy; y <= ey; y++) {
                for (int x = cx; x <= ex; x++) {
                    t = (*level)(x, y);
                    cam->translatePoint(x, y, tx, ty);

                    if (auto p = t.lock()) {
                        int fg = p->getForegroundColor(x, y, level);
                        int bg = p->getBackgroundColor(x, y, level);
                        int c = p->getIconIndex(x, y, level);

                        put(tx, ty, c, fg, bg);
                    }
                }
            }
        }
    }
}
