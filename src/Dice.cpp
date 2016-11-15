#include "dice.hpp"

#include <pcg_random.hpp>
#include <pcg_extras.hpp>
#include <random>

static pcg_extras::seed_seq_from<std::random_device> seed_source;
static pcg32 rng = pcg32(seed_source);

int game::dice::roll(int x, int y) {
    std::uniform_int_distribution<> dist(0, y);
    int num = 0;
    for (int i = 0; i < x; i++)
        num += dist(rng);
    return num;
}
