#ifndef INCLUDE_STRUCTS_HPP_
#define INCLUDE_STRUCTS_HPP_
#include <boost/any.hpp>
#include <map>
#include <easylogging++.h>
#include <json.hpp>
#include <string>

using std::string;
using std::map;
using boost::any;

namespace game {
namespace data {

// Properties class for storing arbitrary data types
class Properties {
public:
    // Constructor
    Properties() {
    }

    Properties(const Properties& c){
        mInternalMap = c.mInternalMap;
    }
    // Destructor
    ~Properties() {
    }

    // Getter
    any rawget(const string& key) {
        return mInternalMap[key];
    }
    // Getter (with cast)
    template<typename Value> Value cast(const string& key,
            const Value& defVal) {
        try {
            return boost::any_cast<Value>(rawget(key));
        } catch (boost::bad_any_cast& e) {
            return defVal;
        }
    }
    // Setter
    template<typename Value> void put(const string& key, const Value& value) {
        mInternalMap[key] = any(value);
    }
    template<typename Value> void put(const string& key, const Value&& value) {
        mInternalMap[key] = any(value);
    }

    // Iterator
    map<string, any>::const_iterator cbegin() const {
        return mInternalMap.cbegin();
    }

    map<string, any>::const_iterator cend() const {
        return mInternalMap.cend();
    }

    map<string, any>::iterator begin() {
        return mInternalMap.begin();
    }

    map<string, any>::iterator end() {
        return mInternalMap.end();
    }
private:
    // Internal storage
    map<string, any> mInternalMap;
};

// Class for objects that have Properties class attached
class PropertyHolder {
public:
    // Constructor
    PropertyHolder() {
    }
    // Destructor
    virtual ~PropertyHolder() {
    }

    // Initializer
    void load(const nlohmann::json& json);

    // Getter
    template<typename Value> Value getProperty(const string& key,
            const Value& defVal) {
        try {
            return boost::any_cast<Value>(properties.rawget(key));
        } catch (boost::bad_any_cast& e) {
            return defVal;
        }
    }

    // Setter
    template<typename Value> void setProperty(const string& key,
            const Value&& value) {
        properties.put(key, std::move(value));
    }
    template<typename Value> void setProperty(const string& key,
            const Value& value) {
        properties.put(key, value);
    }
protected:
    Properties properties;
};

}
}

#endif
