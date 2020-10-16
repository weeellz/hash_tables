#pragma once
#include "d_linked_list.h"
#include "hash_function.h"
#include <iostream>
#include <memory>
#include <vector>

template <class Key, class T, class Hash = hash_function<Key>,
          class Allocator = std::allocator<std::vector<d_linked_list<Key, T>>>>
class separate_chaining {
public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = d_linked_list<Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using hasher = Hash;
  using allocator_type = Allocator;
  using reference = value_type &;
  using const_reference = const value_type &;

private:
  Allocator allocator;
  constexpr static size_t initial_capacity{32};
  // std::pair<Key, T>* data_;
  size_t size_;
  size_t capacity_;
  hash_function<T> hash_function_;

public:
  std::vector<d_linked_list<Key, T>> data_;

  separate_chaining();
  explicit separate_chaining(size_t table_capacity);
  explicit separate_chaining(Allocator allocator);
  ~separate_chaining();
  Key insert(const Key &key, const T &value);
  auto search(const Key &key);
  void erase(const Key &key);
  size_t size() const;
  size_t capacity() const;
};

template <class Key, class T, class Hash, class Allocator>
separate_chaining<Key, T, Hash, Allocator>::separate_chaining()
    : // data_(allocator.allocate(initial_capacity)),
      size_(0), capacity_(initial_capacity), hash_function_(hasher(capacity_)) {
  data_.resize(capacity_);

  /*for (size_t i = 0; i < data_.capacity(); ++i)
  {

  }*/
}

template <class Key, class T, class Hash, class Allocator>
separate_chaining<Key, T, Hash, Allocator>::separate_chaining(
    size_t table_capacity)
    : // data_(allocator.allocate(table_capacity)),
      size_(table_capacity), capacity_(table_capacity),
      hash_function_(hasher(capacity_)) {
  data_.resize(table_capacity);

  /*for (size_t i = 0; i < capacity_; ++i)
  {
          data_[i] = nullptr;
  }*/
  // data_ = new std::vector<d_linked_list<Key, T>>();
  // data_->reserve(table_capacity);
}

template <class Key, class T, class Hash, class Allocator>
separate_chaining<Key, T, Hash, Allocator>::separate_chaining(
    Allocator allocator)
    : allocator(allocator),
      // data_(allocator.allocate(initial_capacity)),
      size_(initial_capacity), capacity_(initial_capacity),
      hash_function_(hasher(capacity_)) {
  data_.resize(capacity_);

  // for (size_t i = 0; i < capacity_; ++i)
  //{
  //	//data_++ = nullptr;
  //}
}

template <class Key, class T, class Hash, class Allocator>
separate_chaining<Key, T, Hash, Allocator>::~separate_chaining() {
  // allocator.deallocate(data_, capacity_);
}

template <class Key, class T, class Hash, class Allocator>
Key separate_chaining<Key, T, Hash, Allocator>::insert(const Key &key,
                                                       const T &value) {
  size_t hash = hash_function_.multiplication_hashing(key);
  data_[hash].insert(std::make_pair(key, value), 0);
  //++size_;
  // std::cout << "Insert successful. Inserted in " << hash << "." << std::endl;
  return hash;
}

template <class Key, class T, class Hash, class Allocator>
auto separate_chaining<Key, T, Hash, Allocator>::search(const Key &key) {
  size_t hash = hash_function_.multiplication_hashing(key);
  return data_[hash].find(key);
}

template <class Key, class T, class Hash, class Allocator>
void separate_chaining<Key, T, Hash, Allocator>::erase(const Key &key) {
  size_t hash = hash_function_.multiplication_hashing(key);
  data_[hash].remove(key);
}

template <class Key, class T, class Hash, class Allocator>
size_t separate_chaining<Key, T, Hash, Allocator>::size() const {
  return size_;
}

template <class Key, class T, class Hash, class Allocator>
size_t separate_chaining<Key, T, Hash, Allocator>::capacity() const {
  return capacity_;
}
