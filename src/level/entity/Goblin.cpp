#include "entities/goblin.hpp"

void game::level::entity::GoblinEntity::onAttackedBy(int dmg,
        AbstractEntity* src) {
}

void game::level::entity::GoblinEntity::onTick(int n) {
    vector<std::tuple<int, int, int>> movCandidates;
    int ex, ey;
    getPosition(ex, ey);

    for (int x = ex - 1; x <= ex + 1; x++) {
        for (int y = ey - 1; y <= ey + 1; y++) {
            int v = mLevel->getPlayerDistance(x, y);
            if (v == 0) {
                LOG(DEBUG)<< "Player at " << x << ", " << y;
                if (canAttack(x, y)) {
                    AbstractEntity* target = mLevel->getEntityAt(x, y);
                    if (target) {
                        target->onAttackedBy(1, this);
                        return;
                    }
                }
            }

            movCandidates.push_back( { x, y, v });
        }
    }

    auto it =
            std::max_element(movCandidates.begin(), movCandidates.end(),
                    [](std::tuple<int, int, int>& t0, std::tuple<int, int, int>& t1)-> int {
                        return std::get<2>(t0) > std::get<2>(t1);
                    });

    if (it != movCandidates.end()) {
        int dx, dy;
        dx = std::get<0>(*it);
        dy = std::get<1>(*it);
        if (checkMove(dx - ex, dy - ey)) {
            move(dx - ex, dy - ey);
        }
    }
}
