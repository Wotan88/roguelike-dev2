#include "generators.hpp"

game::level::gen::BoxGenerator::BoxGenerator() {

}

game::level::gen::BoxGenerator::~BoxGenerator() {

}

void game::level::gen::BoxGenerator::generate(Level* l) {
    string mWallTile = getProperty<string>("wall", "");
    string mFloorTile = getProperty<string>("floor", "");
    for (int x = 0; x < l->getWidth(); x++) {
        for (int y = 0; y < l->getHeight(); y++) {
            if (x == 0 || y == 0 || x == l->getWidth() - 1
                    || y == l->getHeight() - 1)
                l->set(x, y, mWallTile);
            else
                l->set(x, y, mFloorTile);
        }
    }
}
