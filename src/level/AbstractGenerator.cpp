#include "level.hpp"

game::level::AbstractGenerator::AbstractGenerator() {
}

game::level::AbstractGenerator::~AbstractGenerator() {

}

void game::level::AbstractGenerator::generate(
        game::level::Level* l) {

}

void game::level::AbstractGenerator::getSpawnPosition(int& x, int& y){
    x = 0;
    y = 0;
}
