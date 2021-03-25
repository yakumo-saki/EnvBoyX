#pragma once

#include <Arduino.h>

typedef struct simple_map_data_t {
  String key;
  String value;
} simple_map_data_t;

/**
 * Map-Like array
 */
class SimpleMap {

  private:
    static const int NOT_FOUND = -128;

    std::vector<simple_map_data_t> vector;

  public:

    // すべて削除
    void clear() {
      vector.clear();
    }

    // return all keys
    std::vector<String> getKeys() {
      std::vector<String> ret;
      for (auto itr = this->vector.cbegin(); itr == this->vector.cend(); itr++) {
         ret.push_back(itr->key);
      }
      return ret;
    }

    int findIndex(String key) {
      for (int i = 0; i < vector.size(); i++) {
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
    String get(String key) {
      int idx = this->findIndex(key);
      if (idx == NOT_FOUND) return "";
      return vector[idx].value;
    }

    // @param create Create new key or not. 
    // @return success or not.
    bool set(String key, String value, bool create = false) {
      int idx = this->findIndex(key);
      if (idx == NOT_FOUND && !create) return false;
      
      simple_map_data_t data {key, value};
      if (idx == NOT_FOUND) {
        vector.push_back(data);
      } else {
        vector[idx] = data;
      }
      return true;
    }

    // @param failIfExist Fail if key is already exist
    // @return success or not
    bool put(String key, String value, bool failIfExist = false) {
      int idx = this->findIndex(key);
      if (idx != NOT_FOUND && failIfExist) return false;
      
      simple_map_data_t data {key, value};
      if (idx == NOT_FOUND) {
        vector.push_back(data);
      } else {
        vector[idx] = data;
      }
      return true;
    }

};