#include "game.hpp"
#include "gfx.hpp"

#include <easylogging++.h>
#include <SDL2/SDL_image.h>

game::gfx::SpriteSheet::SpriteSheet(const string& filename, int wc, int hc) :
        mFilename(filename), mWidthCells(wc), mHeightCells(hc), mCellWidth(0), mCellHeight(
                0), mWidth(0), mHeight(0), mTexture(nullptr) {
    load();
}

game::gfx::SpriteSheet::~SpriteSheet() {
    LOG(DEBUG)<< "SpriteSheet(\"" << mFilename << "\") object deallocated";

    if (mTexture) {
        SDL_DestroyTexture(mTexture);
    }
}

void game::gfx::SpriteSheet::load() {
    LOG(DEBUG)<< mFilename;

    SDL_Texture* tex = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(mFilename.c_str());
    if(!loadedSurface) {
        LOG(FATAL)<<"Unable to load image! SDL_image Error: "<< IMG_GetError();
    } else {
        LOG(DEBUG)<< "Preparing loaded surface";
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ));
        tex = SDL_CreateTextureFromSurface(GR->sdlRenderer(), loadedSurface);
        LOG(DEBUG)<< "Processing texture";
        if(!tex) {
            LOG(FATAL) << "Failed to create texture! SDL Error: "<<SDL_GetError();
        } else {
            mTexture = tex;
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
            LOG(INFO)<< "Real size: " << mWidth << "x" << mHeight;
            mCellWidth = mWidth / mWidthCells;
            mCellHeight = mHeight / mHeightCells;
        }
        SDL_FreeSurface(loadedSurface);
    }
}

void game::gfx::SpriteSheet::applyColor(int h) {
    SDL_SetTextureColorMod(mTexture, (h >> 16) & 0xFF, (h >> 8) & 0xFF,
            h & 0xFF);
}

void game::gfx::SpriteSheet::render(int i, int x, int y, int w, int h) {
    int tx, ty;
    tx = i % mWidthCells;
    ty = i / mWidthCells;
    SDL_Rect clip { tx * mCellWidth, ty * mCellHeight, mCellWidth, mCellHeight };
    SDL_Rect bounds { x, y, w, h };

    SDL_RenderCopy(GR->sdlRenderer(), mTexture, &clip, &bounds);
}

void game::gfx::SpriteSheet::render(int i, SDL_Rect* r) {
    int tx, ty;
    tx = i % mWidthCells;
    ty = i / mWidthCells;
    SDL_Rect clip { tx * mCellWidth, ty * mCellHeight, mCellWidth, mCellHeight };

    SDL_RenderCopy(GR->sdlRenderer(), mTexture, &clip, r);
}
