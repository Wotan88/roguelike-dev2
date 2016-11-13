#include "registry.hpp"

#include <easylogging++.h>

static int mLastId = 0;
static map<string, int> mIdBindings;
static vector<game::level::AbstractEntity*> mEntites(1024);

game::level::AbstractEntity* game::entityregistry::byId(int id) {
    return mEntites[id];
}

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
        LOG(FATAL)<< "Empty asset name";
    }
    LOG(INFO)<< "Registering "<<assetName;

    auto t1 = game::entityregistry::byName(assetName);
    if (t1) {
        LOG(FATAL)<< "Asset name "<< assetName << " is already used by " << assetName << ":"<<t1->getProperty<int>("id", -1);
    }
    int newId = game::entityregistry::nextId();
    if (newId <= 0 || newId >= 1024)
        LOG(FATAL)<< "ID limit reached";
    LOG(INFO)<< "Binding "<<assetName<<":"<<newId;
    t->setProperty<int>("id", newId);
    mEntites[newId] = t;
    mIdBindings[assetName] = newId;
}
