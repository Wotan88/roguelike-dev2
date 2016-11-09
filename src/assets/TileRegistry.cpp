#include "registry.hpp"

#include <easylogging++.h>

static int mLastId = 0;
static map<string, int> mIdBindings;
static vector<std::shared_ptr<game::level::AbstractTile>> mTiles(1024);

std::weak_ptr<game::level::AbstractTile> game::tileregistry::byId(int id) {
    std::weak_ptr<game::level::AbstractTile> t(mTiles[id]);
    return t;
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

std::weak_ptr<game::level::AbstractTile> game::tileregistry::byName(
        const std::string& name) {
    static std::weak_ptr<game::level::AbstractTile> null_tile_ptr;
    map<string, int>::iterator it = mIdBindings.find(name);
    if (it == mIdBindings.end()) {
        return null_tile_ptr;
    }
    return std::weak_ptr<game::level::AbstractTile>(mTiles[(*it).second]);
}

int game::tileregistry::nextId() {
    return ++mLastId;
}

void game::tileregistry::bind(std::shared_ptr<game::level::AbstractTile> t) {
    if (!t)
        return;
    std::string assetName = t->getProperty<string>("assetName", "");
    if (assetName.empty()) {
        LOG(FATAL)<< "game::tileregistry::registerTile(): tile has empty asset name";
    }
    LOG(INFO)<< "Registering tile "<<assetName;

    auto t1 = game::tileregistry::byName(assetName);
    if (auto p = t1.lock()) {
        LOG(FATAL)<< "game::tileregistry::registerTile(): asset name "<< assetName << " is already used by " << assetName << ":"<<p->getId();
    } else {
        LOG(DEBUG) << "auto p = t1.lock() == nullptr";
    }

    int newId = game::tileregistry::nextId();

    if (newId <= 0 || newId >= 1024)
        LOG(FATAL)<< "game::tileregistry::registerTile(): tile ID limit reached";

    LOG(INFO)<< "Binding "<<assetName<<":"<<newId;

    t->setId(newId);
    mTiles[newId] = t;
    mIdBindings[assetName] = newId;
}
