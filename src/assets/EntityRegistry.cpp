#include "registry.hpp"

#include <easylogging++.h>

static int mLastId = 0;
static map<string, int> mIdBindings;
static vector<game::level::AbstractEntity*> mEntites(1024);

game::level::AbstractEntity* game::entityregistry::byId(int id) {
    return mEntites[id];
}

//std::vector<std::shared_ptr<game::level::AbstractTile>>::iterator t =
//        std::find_if(mTiles.begin(), mTiles.end(),
//                [&name](std::shared_ptr<game::level::AbstractTile> t) -> bool {
//                    if (t && t->getAssetName() == name) {
//                        return true;
//                    }
//                    return false;
//                });
//if (t == mTiles.end()) {
//    return null_tile_ptr;
//};

game::level::AbstractEntity* game::entityregistry::byName(
        const std::string& name) {
    map<string, int>::iterator it = mIdBindings.find(name);
    if (it == mIdBindings.end()) {
        return nullptr;
    }
    return mEntites[(*it).second];
}

int game::entityregistry::nextId() {
    return ++mLastId;
}

void game::entityregistry::bind(game::level::AbstractEntity* t) {
    if (!t)
        return;
    std::string assetName = t->getProperty<string>("assetName", "");
    if (assetName.empty()) {
        LOG(FATAL)<< "game::tileregistry::registerTile(): tile has empty asset name";
    }
    LOG(INFO)<< "Registering tile "<<assetName;

    auto t1 = game::entityregistry::byName(assetName);
    if (t1) {
        LOG(FATAL)<< "game::tileregistry::registerTile(): asset name "<< assetName << " is already used by " << assetName << ":"<<t1->getProperty<int>("id", -1);
    } else {
        LOG(DEBUG) << "auto p = t1.lock() == nullptr";
    }

    int newId = game::entityregistry::nextId();

    if (newId <= 0 || newId >= 1024)
        LOG(FATAL)<< "game::tileregistry::registerTile(): tile ID limit reached";

    LOG(INFO)<< "Binding "<<assetName<<":"<<newId;

    t->setProperty<int>("id", newId);
    mEntites[newId] = t;
    mIdBindings[assetName] = newId;
}