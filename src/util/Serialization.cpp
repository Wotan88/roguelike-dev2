#include "serialization.hpp"
#include "general_utils.hpp"
#include "structs.hpp"
#include "registry.hpp"
#include "tiles/all.hpp"
#include "entities/all.hpp"
#include "items/all.hpp"

#include <json.hpp>

// Class instantiators

game::level::AbstractTile* game::serialization::instantiateTileClass(
        const string& basename) {
    if (basename == "Door")
        return new game::level::tile::Door();

    return new game::level::AbstractTile();
}

game::level::AbstractEntity* game::serialization::instantiateEntityClass(
        const string& basename) {
    if (basename == "Goblin") {
        return new level::entity::GoblinEntity(nullptr);
    }
    return new level::AbstractEntity(nullptr);
}

game::item::AbstractItem* game::serialization::instantiateItemClass(
        const string& basename) {
    if (basename == "Food") {
        return new item::FoodItem();
    }
    return new item::AbstractItem();
}

//

void game::data::PropertyHolder::load(const nlohmann::json& json) {
    for (auto it = json.begin(); it != json.end(); it++) {
        auto v = it.value();
        if (v.is_null())
            continue;

        // int
        if (v.is_number_integer()) {
            setProperty<int>(it.key(), v.get<int>());
            continue;
        }

        // string
        if (v.is_string()) {
            string vs = v.get<string>();

            // color
            if (vs.length() > 0 && (vs[0] == '#')) {
                int col = game::util::parseColor(vs);
                if (col >= 0) {
                    setProperty<int>(it.key(), col);
                    continue;
                }
            }
            setProperty<string>(it.key(), v.get<string>());
            continue;
        }

        // bool
        if (v.is_boolean()) {
            setProperty<bool>(it.key(), v.get<bool>());
            continue;
        }

    }
}

game::level::AbstractTile* game::serialization::loadTile(
        const string& filename) {
    LOG(DEBUG)<< filename;
    if (!game::util::fileExists(filename))
    return nullptr;

    std::ifstream jsonInputStream(filename);
    nlohmann::json jsonData = nlohmann::json::parse(jsonInputStream);

    string classname = "AbstractTile";
    if (!jsonData["class"].is_null()) {
        classname = jsonData["class"].get<string>();
    }

    jsonData["class"] = classname;

    for (nlohmann::json::iterator it = jsonData.begin(); it != jsonData.end(); it++) {
        // Starts with icon and is a string
        if (it.value().is_string() && it.key().find("icon") == 0) {
            auto v = it.value().get<string>();
            if (v.length() == 1) {
                jsonData[it.key()] = (int)(v[0]);
            }
        }
    }

    game::level::AbstractTile* tObj = instantiateTileClass(
            classname);
    tObj->load(jsonData);

    return tObj;
}

game::item::AbstractItem* game::serialization::loadItem(
        const string& filename) {
    LOG(DEBUG)<< filename;
    if (!game::util::fileExists(filename))
    return nullptr;

    std::ifstream jsonInputStream(filename);
    nlohmann::json jsonData = nlohmann::json::parse(jsonInputStream);

    string classname = "AbstractItem";
    if (!jsonData["class"].is_null()) {
        classname = jsonData["class"].get<string>();
    }

    jsonData["class"] = classname;

    for (nlohmann::json::iterator it = jsonData.begin(); it != jsonData.end(); it++) {
        // Starts with icon and is a string
        if (it.value().is_string() && it.key().find("icon") == 0) {
            auto v = it.value().get<string>();
            if (v.length() == 1) {
                jsonData[it.key()] = (int)(v[0]);
            }
        }
    }

    game::item::AbstractItem* iObj = instantiateItemClass(classname);
    iObj->load(jsonData);

    return iObj;
}

game::level::AbstractEntity* game::serialization::loadEntity(
        const string& filename) {
    LOG(DEBUG)<< filename;
    if (!game::util::fileExists(filename))
    return nullptr;

    std::ifstream jsonInputStream(filename);
    nlohmann::json jsonData = nlohmann::json::parse(jsonInputStream);

    string classname = "AbstractEntity";
    if (!jsonData["class"].is_null()) {
        classname = jsonData["class"].get<string>();
    }

    jsonData["class"] = classname;

    for (nlohmann::json::iterator it = jsonData.begin(); it != jsonData.end(); it++) {
        // Starts with icon and is a string
        if (it.value().is_string() && it.key().find("icon") == 0) {
            auto v = it.value().get<string>();
            if (v.length() == 1) {
                jsonData[it.key()] = (int)(v[0]);
            }
        }
    }

    game::level::AbstractEntity* iObj = instantiateEntityClass(classname);
    iObj->load(jsonData);

    return iObj;
}

void game::serialization::loadAllTiles(const string& dirn) {
    vector<string> files = game::util::listDir(dirn);
    for (auto f : files) {
        if (f[0] != '.') {
            auto t = loadTile(util::join(dirn, f));
            tileregistry::bind(t);
        }
    }
}

void game::serialization::loadAllItems(const string& dirn) {
    vector<string> files = game::util::listDir(dirn);
    for (auto f : files) {
        if (f[0] != '.') {
            auto t = loadItem(util::join(dirn, f));
            itemregistry::bind(t);
        }
    }
}

void game::serialization::loadAllEntities(const string& dirn) {
    vector<string> files = game::util::listDir(dirn);
    for (auto f : files) {
        if (f[0] != '.') {
            auto t = loadEntity(util::join(dirn, f));
            entityregistry::bind(t);
        }
    }
}
