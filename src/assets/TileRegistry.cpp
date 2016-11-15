#include "registry.hpp"

#include <easylogging++.h>

static int mLastId = 0;
static map<string, int> mIdBindings;
static vector<game::level::AbstractTile*> mTiles(1024);

game::level::AbstractTile* game::tileregistry::byId(int id) {
    return mTiles[id];
}

game::level::AbstractTile* game::tileregistry::byName(const std::string& name) {
    map<string, int>::iterator it = mIdBindings.find(name);
    if (it == mIdBindings.end()) {
        return nullptr;
    }
    return mTiles[(*it).second];
}

int game::tileregistry::nextId() {
    return ++mLastId;
}

map<string, int>& game::tileregistry::getMappingTable(){
    return mIdBindings;
}

void game::tileregistry::bind(game::level::AbstractTile* t) {
    if (!t)
        return;
    std::string assetName = t->getProperty<string>("assetName", "");
    if (assetName.empty()) {
        LOG(FATAL)<< "Empty asset name";
    }
    LOG(INFO)<< "Registering tile "<<assetName;

    auto t1 = game::tileregistry::byName(assetName);
    if (t1) {
        LOG(FATAL)<< "Asset name "<< assetName << " is already used by " << assetName << ":"<<t1->getId();
    }
    int newId = game::tileregistry::nextId();
    if (newId <= 0 || newId >= 1024)
        LOG(FATAL)<< "ID limit reached";
    LOG(INFO)<< "Binding "<<assetName<<":"<<newId;
    t->setId(newId);
    mTiles[newId] = t;
    mIdBindings[assetName] = newId;
}
