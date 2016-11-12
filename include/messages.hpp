#ifndef INCLUDE_MESSAGES_HPP_
#define INCLUDE_MESSAGES_HPP_
#include <string>
#include <queue>

namespace game {
namespace messages {
void push(const std::string& msg);
std::string pop();
bool next();
}
}

#endif
