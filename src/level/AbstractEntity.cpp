#include "entity.hpp"

game::level::AbstractEntity::AbstractEntity(Level* l) :
        mX(0), mY(0), mLevel(l) {
    setProperty<string>("class", "AbstractEntity");
}

game::level::AbstractEntity::~AbstractEntity() {
    LOG(DEBUG)<< "Entity destroyed";
}

int game::level::AbstractEntity::getForegroundColor() {
    return getProperty<int>("foregroundColor", 0xFFFF00);
}

int game::level::AbstractEntity::getIconIndex() {
    return getProperty<int>("iconIndex", '@');
}

int game::level::AbstractEntity::x() {
    return mX;
}

int game::level::AbstractEntity::y() {
    return mY;
}

void game::level::AbstractEntity::getPosition(int& x, int& y) {
    x = mX;
    y = mY;
}

void game::level::AbstractEntity::setPosition(int x, int y) {
    mX = x;
    mY = y;
}

void game::level::AbstractEntity::move(int dx, int dy) {
    mX += dx;
    mY += dy;
}

bool game::level::AbstractEntity::checkMove(int dx, int dy) {
    int nx = mX + dx;
    int ny = mY + dy;

    if (nx < 0 || ny < 0 || nx >= mLevel->getWidth()
            || ny >= mLevel->getHeight())
        return false;

    AbstractTile* t = (*mLevel)(nx, ny);

    if (!t)
        return false;
    return !t->isCollidable(nx, ny, mLevel);
}

bool game::level::AbstractEntity::onCollideTile(int x, int y) {
    return false;
}

void game::level::AbstractEntity::setLevel(Level* l){
    mLevel = l;
}
