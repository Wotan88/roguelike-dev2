#include "messages.hpp"

static std::queue<std::string> mQueue;

void game::messages::push(const std::string& msg) {
    mQueue.push(msg);
}

std::string game::messages::pop() {
    std::string e = mQueue.front();
    mQueue.pop();
    return e;
}

bool game::messages::next() {
    return !mQueue.empty();
}
