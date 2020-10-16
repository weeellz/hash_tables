#pragma once
#include "hash_function.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

template <class Key, class T, class Hash1 = std::hash<Key>,
          class Hash2 = std::hash<Key>>
class open_addressing_map {
private:
  enum CellState : char {
    EMPTY = 1,    // 00000001
    DELETED = 2,  // 00000010
    OCCUPIED = 4, // 00000100
  };

  static constexpr size_t initial_capacity{32};
  Hash1 hash1_;
  Hash2 hash2_;
  std::vector<std::pair<Key, T>> data_;
  std::vector<CellState> states_;

  size_t double_hashing(Key key, size_t i) {
    auto h1 = hash1_(key) % data_.size();
    auto h2 = (1 + 2 * hash2_(key)) % data_.size();
    return (h1 + i * h2) % data_.size();
  }

public:
  open_addressing_map(size_t table_capacity = initial_capacity,
                      Hash1 hash1 = Hash1(), Hash2 hash2 = Hash2())
      : data_(table_capacity), states_(table_capacity, EMPTY), hash1_(hash1),
        hash2_(hash2) {}

  bool insert(const Key &key, const T &value) {
    for (size_t i = 0; i < data_.size(); ++i) {
      size_t hash = double_hashing(key, i);
      if (states_[hash] & (EMPTY | DELETED)) {
        data_[hash].first = key;
        data_[hash].second = value;
        states_[hash] = OCCUPIED;
        return true;
      }
    }
    return false;
  }

  std::pair<Key, T> *search(const Key &key) {
    for (size_t i = 0; i < data_.size(); ++i) {
      size_t hash = double_hashing(key, i);
      if (states_[hash] == EMPTY) {
        return nullptr;
      }
      if (data_[hash].first == key) {
        return data_.data() + hash;
      }
    }
    return nullptr;
  }

  void erase(const Key &key) {
    for (size_t i = 0; i < data_.size(); ++i) {
      size_t hash = double_hashing(key, i);
      if (data_[hash].first == key) {
        states_[hash] = DELETED;
        return;
      }
    }
  }

  void print() {
    for (int i = 0; i < data_.size(); ++i) {
      if (states_[i] == OCCUPIED) {
        std::cout << data_[i].first;
        std::cout << " ";
        std::cout << data_[i].second;
        std::cout << std::endl;
      }
    }
  }
};
