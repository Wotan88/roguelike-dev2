#include "easylogging++.h"
#include "game.hpp"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv){
    LOG(INFO)<< "Starting";

    game::Game::start();
    return 0;
}
