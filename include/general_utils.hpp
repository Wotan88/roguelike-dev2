#ifndef INCLUDE_GENERAL_UTILS_HPP_
#define INCLUDE_GENERAL_UTILS_HPP_
#include <vector>
#include <string>

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif


using std::vector;
using std::string;

namespace game {
namespace util {

// Combines two paths (p1 is parent, p2 is child)
std::string join(const std::string& p1, const std::string& p2);

// Checks if file exists
bool fileExists(const string& path);

// Converts string to color
int parseColor(const string& str);

vector<string> listDir(const string& str);

// Returns value of key with 'shift' held
int shiftKey(int k);

// Darkens color by `v` tenths
int darken(int col, int v);

// Performs word-wrapping function
string wrap(const string& in, int width);
}
}

#endif
