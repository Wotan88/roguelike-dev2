#include "camera.hpp"

game::level::Camera::Camera(int x, int y) :
        mX(x), mY(y) {
}

game::level::Camera::~Camera() {

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
