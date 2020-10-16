#pragma once
#include <unordered_set>
#include <vector>
template <class Key, class T, class Hash = std::hash<Key>>
class perfect_hashing_map {
  struct uni_hash {
    size_t p = 638053337; // 3571 или 8950186633 long long или 638053337 int
    size_t a;
    size_t b;
    Hash *hash;

    explicit uni_hash(Hash *hash) : hash(hash) { update(); }

    uni_hash() : hash(nullptr) { update(); }

    size_t operator()(const Key &k) { return ((a * (*hash)(k) + b) % p); }

    void update() {
      a = 1 + rand() % (p - 1);
      b = 0 + rand() % p;
    }

    void zeros() {
      a = 0;
      b = 0;
    }

    bool check(const std::vector<Key> &keys) {
      std::unordered_set<size_t> hashes;

      for (auto &key : keys) {
        hashes.insert(operator()(key) % (keys.size() * keys.size()));
      }
      return hashes.size() == keys.size();
    }
  };
  Hash hash_;
  std::vector<
      std::pair<std::vector<std::pair<bool, std::pair<Key, T>>>, uni_hash>>
      data_;

public:
  size_t n_elements = 0;
  perfect_hashing_map(const std::vector<Key> &keys, Hash hash = Hash())
      : hash_(hash), data_(keys.size()) {
    std::vector<std::vector<Key>> keys_to_row(keys.size());
    for (auto &key : keys) {
      keys_to_row[hash_(key) % keys.size()].push_back(key);
    }

    for (size_t i = 0; i < data_.size(); ++i) {
      data_[i].first.resize(keys_to_row[i].size() * keys_to_row[i].size());
      data_[i].second.hash = &hash_;

      while (!data_[i].second.check(keys_to_row[i])) {
        data_[i].second.update();
      }

      for (size_t j = 0; j < data_[i].first.size(); ++j) {
        // occupied = true
        data_[i].first[j].first = false;
      }
    }
  }

  bool insert(const Key &key, const T &value) {
    auto found = search(key);
    if (found != nullptr) {
      found->second += 1;
      return false;
    }
    // std::vector<std::pair<std::vector<std::pair<bool, std::pair<Key, T>>>,
    // uni_hash>> data_;
    auto external_table_pos = hash_(key) % data_.size();
    auto internal_table_pos = data_[external_table_pos].second(key) %
                              data_[external_table_pos].first.size();
    if (data_[external_table_pos].first[internal_table_pos].first) {
      return false;
    }
    data_[external_table_pos].first[internal_table_pos].first =
        true; // occupied = true
    data_[external_table_pos].first[internal_table_pos].second.first = key;
    data_[external_table_pos].first[internal_table_pos].second.second = value;
    ++n_elements;
    return true;
  }

  std::pair<Key, T> *search(const Key &key) {
    auto external_table_pos = hash_(key) % data_.size();
    auto internal_table_pos = data_[external_table_pos].second(key) %
                              data_[external_table_pos].first.size();
    if (data_[external_table_pos].first[internal_table_pos].first) {
      return &data_[external_table_pos].first[internal_table_pos].second;
    }
    return nullptr;
  }

  void erase(const Key &key) {
    auto external_table_pos = hash_(key) % data_.size();
    auto internal_table_pos = data_[external_table_pos].second(key) %
                              data_[external_table_pos].first.size();
    data_[external_table_pos].first[internal_table_pos].first =
        false; // occupied = false
    --n_elements;
  }

  // std::vector<std::pair<std::vector<std::pair<bool, std::pair<Key, T>>>,
  // uni_hash>> data_;
  void print() {
    for (size_t i = 0; i < data_.size(); ++i) {
      for (size_t j = 0; j < data_[i].first.size(); ++j) {
        if (data_[i].first[j].first) {
          std::cout << data_[i].first[j].second.first << " "
                    << data_[i].first[j].second.second << std::endl;
        }
      }
    }
  }
};