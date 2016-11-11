#include "level.hpp"

static std::vector<game::level::Level*> mLevels;
static int lastDepth = 0;

void game::level::depths::push(Level* l) {
    int nn = lastDepth++;
    mLevels.resize(lastDepth);

    l->setProperty<int>("depth", nn);
    mLevels[nn] = l;
}

game::level::Level* game::level::depths::get(int d){
    if (d < 0 || d >= lastDepth)
        return nullptr;

    return mLevels[d];
}

game::level::Level* game::level::depths::last(){
    return mLevels[lastDepth-1];
}

int game::level::depths::count(){
    return lastDepth;
}
