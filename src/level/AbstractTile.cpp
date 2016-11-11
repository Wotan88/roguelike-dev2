#include "level.hpp"

#include <easylogging++.h>

game::level::AbstractTile::AbstractTile() {
//    mAssetName = "";
    mId = 0;
}

game::level::AbstractTile::~AbstractTile() {
    LOG(DEBUG)<< "Tile destroyed";
}

//const string& game::level::AbstractTile::getAssetName() {
//    return mAssetName;
//}

int game::level::AbstractTile::getId() {
    return mId;
}

void game::level::AbstractTile::setId(int newId) {
    mId = newId;
}

//void game::level::AbstractTile::setAssetName(const string& newAssetName) {
//    mAssetName = newAssetName;
//}

int game::level::AbstractTile::getBackgroundColor(int x, int y, Level* l) {
    return getProperty<int>("backgroundColor", 0);
}
int game::level::AbstractTile::getForegroundColor(int x, int y, Level* l) {
    return getProperty<int>("foregroundColor", 0xFFFFFF);
}
int game::level::AbstractTile::getIconIndex(int x, int y, Level* l) {
    return getProperty<int>("iconIndex", 'X');
}

bool game::level::AbstractTile::isCollidable(int x, int y, Level* l){
    return getProperty<bool>("collidable", true);
}

bool game::level::AbstractTile::interactOnCollide(int x, int y, Level* l){
    return false;
}

bool game::level::AbstractTile::onInteract(int x, int y, Level* l, AbstractEntity* src){
    return false;
}

bool game::level::AbstractTile::isDownstairs(int x, int y, Level* l){
    return getProperty<bool>("downstairs", false);
}

bool game::level::AbstractTile::isUpstairs(int x, int y, Level* l){
    return getProperty<bool>("upstairs", false);
}

