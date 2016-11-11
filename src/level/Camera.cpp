#include "camera.hpp"
#include "gfx.hpp"

#include <easylogging++.h>

game::level::Camera::Camera(int x, int y) :
        mX(x), mY(y) {
}

game::level::Camera::~Camera() {
    LOG(DEBUG)<< "Camera destroyed";
}

void game::level::Camera::setPosition(int x, int y) {
    mX = x;
    mY = y;
}

void game::level::Camera::translatePoint(int sx, int sy, int& x, int& y) {
    x = sx - mX;
    y = sy - mY;
}

void game::level::Camera::getPosition(int& x, int& y) {
    x = mX;
    y = mY;
}

void game::level::Camera::move(int x, int y) {
    mX += x;
    mY += y;
}

void game::level::Camera::center(int x, int y) {
    mX = x - game::gfx::SCREEN_WIDTH / 2;
    mY = y - game::gfx::SCREEN_HEIGHT / 2;
}
