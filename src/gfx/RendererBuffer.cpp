#include "gfx.hpp"

game::gfx::RendererBuffer::RendererBuffer(int w, int h) {
    mInternalBuffer = vector<CharacterWrapper>(w * h);
    mWidth = w;
    mHeight = h;
}

game::gfx::RendererBuffer::~RendererBuffer() {
}

void game::gfx::RendererBuffer::put(int x, int y, int c, int f, int b) {
    mInternalBuffer[x + y * mWidth] = CharacterWrapper { c, b, f };
}

game::gfx::CharacterWrapper& game::gfx::RendererBuffer::operator ()(int x,
        int y) {
    return mInternalBuffer[x + y * mWidth];
}

void game::gfx::RendererBuffer::clear() {
    static const CharacterWrapper cw { 0, 0, 0 };
    std::fill(mInternalBuffer.begin(), mInternalBuffer.end(), cw);
}
