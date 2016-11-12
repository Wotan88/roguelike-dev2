#include "entity.hpp"
#include "serialization.hpp"

game::level::AbstractEntity::AbstractEntity(Level* l) :
        mX(0), mY(0), mLevel(l) {
    setProperty<string>("class", "AbstractEntity");
}

game::level::AbstractEntity* game::level::AbstractEntity::clone() {
    LOG(DEBUG)<< "Clone() " << getProperty<string>("assetName", "");
    LOG(DEBUG)<< getProperty<string>("class", "");
    AbstractEntity* ret = game::serialization::instantiateEntityClass(getProperty<string>("class", ""));
    ret->mLevel = mLevel;
    getPosition(ret->mX, ret->mY);
    ret->properties = this->properties; // Clone properties
    LOG(DEBUG)<< ret->getProperty<string>("class", "");
    return ret;
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
    int px, py;
    px = mX;
    py = mY;
    mX = x;
    mY = y;
    if (mLevel) {
        mLevel->onEntityMoved(px, py, mX, mY);
    }
}

void game::level::AbstractEntity::move(int dx, int dy) {
    mX += dx;
    mY += dy;
    if (mLevel) {
        mLevel->onEntityMoved(mX - dx, mY - dy, mX, mY);
    }
}

bool game::level::AbstractEntity::checkMove(int dx, int dy) {
    int nx = mX + dx;
    int ny = mY + dy;

    if (nx < 0 || ny < 0 || nx >= mLevel->getWidth()
            || ny >= mLevel->getHeight())
        return false;

    AbstractEntity* e = mLevel->getEntityAt(nx, ny);
    if (e)
        return false;

    AbstractTile* t = (*mLevel)(nx, ny);

    if (!t)
        return false;

    return !t->isCollidable(nx, ny, mLevel);
}

bool game::level::AbstractEntity::onCollideTile(int x, int y) {
    return false;
}

void game::level::AbstractEntity::setLevel(Level* l) {
    mLevel = l;
}

bool game::level::AbstractEntity::canAttack(int dx, int dy) {
    AbstractEntity* e = mLevel->getEntityAt(dx, dy);
    return e != nullptr;
}

void game::level::AbstractEntity::onAttackedBy(int dmg, AbstractEntity* src) {
    int hp = this->getProperty<int>("hp", -1);
    if (hp != -1) {
        this->setProperty<int>("hp", hp - dmg);
    }
}

void game::level::AbstractEntity::onTick(int n) {
}
