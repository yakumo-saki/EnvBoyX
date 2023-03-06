#pragma once

#include <Arduino.h>
#include <vector>

template <typename T>
struct SimpleMapData {
  String key;
  T value;
};

/**
 * Map-Like array. 
 * NOTE: Key is always string.
 * typename T is typename of Data
 */
template <typename T> class SimpleMap {

  private:
    static const int NOT_FOUND = -128;

    std::vector<SimpleMapData<T>> values;

    /**
     * Returns index of key.
     * @return index of key. if not found return NOT_FOUND (-128)
     */
    int findIndex(String key) {
      for (unsigned int i = 0; i < values.size(); i++) {
        // debuglog("NOW=" + vector[i].key + " target=" + key);
        if (values[i].key == key) return i;
      }
      return NOT_FOUND;
    }

  public:

    // すべて削除
    void clear() {
      values.clear();
    }

    /** return all keys in map */
    std::vector<String> getKeys() {
      std::vector<String> ret;
      // debuglog("SimpleMap keys start");
      for (auto itr : values) {
          // debuglog("config creation add key=" + itr.key);
          ret.push_back(itr.key);
      }
      return ret;
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
      return values[idx].value;
    }

    // @param create Create new key or not. 
    // @return success or not.
    bool set(String key, const T& value, bool create = false) {
      int idx = this->findIndex(key);
      if (idx == NOT_FOUND && !create) {
        // cfglog("[SimpleMap] KEY=" + key + " not found, and create is false");
        return false;
      }
      
      SimpleMapData<T> data {key, value};
      if (idx == NOT_FOUND) {
        values.push_back(data);
      } else {
        values[idx] = data;
      }

      // debuglog("[SimpleMap] SET KEY=" + key + " value=" + value);

      return true;
    }

    /**
     * put new item to map.
     * @param failIfExist true -> Fail if key is already exist. false -> overwrite
     * @return success or not
     */
    bool put(String key, const T& value, bool failIfExist = false) {
      int idx = this->findIndex(key);
      if (idx != NOT_FOUND && failIfExist) return false;
      
      SimpleMapData<T> data {key, value};
      if (idx == NOT_FOUND) {
        values.push_back(data);
      } else {
        values[idx] = data;
      }
      return true;
    }

    // remove key from map.
    // @return success or not (not means key not found)
    bool remove(String key) {

      for (auto it = values.begin(); it != values.end(); ++it) {
        if (it->key == key) {
          values.erase(it);
          return true;
        }
      }
      
      return false;
    }

};