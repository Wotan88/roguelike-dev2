#include "general_utils.hpp"
#include "structs.hpp"

#include <boost/any.hpp>
#include <sys/stat.h>
#include <dirent.h>

bool game::util::fileExists(const string& path) {
    static struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

int game::util::parseColor(const std::string& in) {
    std::string tmp;
    if (in[0] == '#') {
        tmp = in.substr(1, in.length() - 1);
    }
    if (in[0] == '0' && in[1] == 'x') {
        tmp = in.substr(2, in.length() - 2);
    }

    char* err;
    long i = strtol(tmp.c_str(), &err, 16);

    if (err && err[0]) {
        return -1;
    } else
        return (int) i;
}

vector<string> game::util::listDir(const string& str) {
    vector<string> ret;
    DIR * dir;
    struct dirent *ent;
    if ((dir = opendir(str.c_str())) != nullptr) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != nullptr) {
            ret.push_back(string { ent->d_name });
        }
        closedir(dir);
    }
    return ret;
}

std::string game::util::join(const std::string& p1, const std::string& p2) {
    std::string tmp;
    if (p1[p1.length() - 1] != PATH_SEPARATOR) {
        tmp = p1 + std::string(1, PATH_SEPARATOR);
    } else
        tmp = p1;
    if (p2[0] == PATH_SEPARATOR) {
        return p1 + p2.substr(1, p2.length() - 1);
    } else
        return p1 + p2;
}
