#pragma once

#include <Arduino.h>
#include "log.h"

template <typename T>
struct SimpleMapData {
  String key;
  T value;
};

/**
 * Map-Like array
 */
template <typename T> class SimpleMap {

  private:
    static const int NOT_FOUND = -128;

    std::vector<SimpleMapData<T>> vector;

  public:

    // すべて削除
    void clear() {
      vector.clear();
    }

    // return all keys
    std::vector<String> getKeys() {
      std::vector<String> ret;
      // debuglog("SimpleMap keys start");
      for (auto itr : vector) {
          // debuglog("config creation add key=" + itr.key);
          ret.push_back(itr.key);
      }
      return ret;
    }

    int findIndex(String key) {
      for (int i = 0; i < vector.size(); i++) {
        // debuglog("NOW=" + vector[i].key + " target=" + key);
        if (vector[i].key == key) return i;
      }
      return NOT_FOUND;
    }

    bool hasKey(String key) {
      return (findIndex(key) != NOT_FOUND);
    }

    /**
     * @return value or null
     */
    T get(String key) {
      int idx = this->findIndex(key);
      if (idx == NOT_FOUND) {
        T temp;
        return temp;
      }
      return vector[idx].value;
    }

    // @param create Create new key or not. 
    // @return success or not.
    bool set(String key, const T& value, bool create = false) {
      int idx = this->findIndex(key);
      if (idx == NOT_FOUND && !create) {
        cfglog("[SimpleMap] KEY=" + key + " not found, and create is false");
        return false;
      }
      
      SimpleMapData<T> data {key, value};
      if (idx == NOT_FOUND) {
        vector.push_back(data);
      } else {
        vector[idx] = data;
      }

      // debuglog("[SimpleMap] SET KEY=" + key + " value=" + value);

      return true;
    }

    // @param failIfExist Fail if key is already exist
    // @return success or not
    bool put(String key, const T& value, bool failIfExist = false) {
      int idx = this->findIndex(key);
      if (idx != NOT_FOUND && failIfExist) return false;
      
      SimpleMapData<T> data {key, value};
      if (idx == NOT_FOUND) {
        vector.push_back(data);
      } else {
        vector[idx] = data;
      }
      return true;
    }

};