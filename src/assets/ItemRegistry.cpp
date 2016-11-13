#include "registry.hpp"

#include <easylogging++.h>

static int mLastId = 0;
static map<string, int> mIdBindings;
static vector<game::item::AbstractItem*> mItems(1024);

game::item::AbstractItem* game::itemregistry::byId(int id) {
    return mItems[id];
}

game::item::AbstractItem* game::itemregistry::byName(const std::string& name) {
    map<string, int>::iterator it = mIdBindings.find(name);
    if (it == mIdBindings.end()) {
        return nullptr;
    }
    return mItems[(*it).second];
}

int game::itemregistry::nextId() {
    return ++mLastId;
}

void game::itemregistry::bind(game::item::AbstractItem* t) {
    if (!t){
        return;
    }
    std::string assetName = t->getProperty<string>("assetName", "");
    if (assetName.empty()) {
        LOG(FATAL)<< "game::itemregistry::registerTile(): tile has empty asset name";
    }
    LOG(INFO)<< "Registering tile "<<assetName;

    auto t1 = game::itemregistry::byName(assetName);
    if (t1) {
        LOG(FATAL)<< "game::itemregistry::registerTile(): asset name "<< assetName << " is already used by " << assetName << ":"<<t1->getProperty<int>("id", -1);
    } else {
        LOG(DEBUG) << "auto p = t1.lock() == nullptr";
    }

    int newId = game::itemregistry::nextId();

    if (newId <= 0 || newId >= 1024)
        LOG(FATAL)<< "game::itemregistry::registerTile(): tile ID limit reached";

    LOG(INFO)<< "Binding "<<assetName<<":"<<newId;

    t->setProperty<int>("id", newId);
    mItems[newId] = t;
    mIdBindings[assetName] = newId;
}
