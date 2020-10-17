#pragma once
#include <memory>
#include <iostream>
#include "hash_function.h"
#include <cassert>
#include <optional>
#include <vector>
#include <string>

namespace hash_tables {

namespace internal {
	template <typename T, bool is_const>
	class iterator {
	public:
		using iterator_category	= std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using size_type = std::size_t;
		/* depending on constancy we must have different reference and pointer types */
		using reference = typename std::conditional<is_const, const value_type &, value_type &>::type;
		using pointer = typename std::conditional<is_const, const value_type *, value_type *>::type;

		/* to enable connection between const and non-const implementations */
		friend class iterator<T, true>;
		friend class iterator<T, false>;

		iterator(std::vector<reference> table, size_type index) {
			_table = &table;
			idx = index;
		}

		/* increment & decrement operators */
		iterator &operator++() {
			++idx;
			return *this;
		}
		iterator &operator++(int) {
			iterator tmp(*_table, idx);
			++idx;
			return tmp;
		}
		iterator &operator--() {
			--idx;
			return *this;
		}
		iterator &operator--(int) {
			iterator tmp(*_table, idx);
			--idx;
			return tmp;
		}
		reference operator[](size_type index) {
			return _table[index];
		}
		/* access operators */
		reference operator*() const {
			return _table[idx];
		}
		pointer operator->() const {
			return &_table[idx];
		}
		/* compare operators */
		/**
		* Template parameter is needed to enable this methods work with
		* non-constant and constant iterators
		*/
		template <bool C>
		bool operator==(const iterator<T, C> &rhs) const {
			return idx == rhs.idx;
		}
		template <bool C>
		bool operator!=(const iterator<T, C> &rhs) const {
			return idx != rhs.idx;
		}
		template <bool C>
		bool operator<(const iterator<T, C> &rhs) const {
			return idx < rhs.idx;
		}
		template <bool C>
		bool operator>(const iterator<T, C> &rhs) const {
			return idx > rhs.idx;
		}
		template <bool C>
		bool operator<=(const iterator<T, C> &rhs) const {
			return idx <= rhs.idx;
		}
		template <bool C>
		bool operator>=(const iterator<T, C> &rhs) const {
			return idx >= rhs.idx;
		}

	private:
		std::vector<T> *_table; /* pointer to underlying vector */
		size_type idx; /* index for current iterator position */

	};
}

template<class Key, class T, class Hash1 = std::hash<Key>, class Hash2 = std::hash<Key>>
class open_addressing_map
{
public:
	using iterator = internal::iterator<std::pair<Key, T>, false>;
	using const_iterator = internal::iterator<std::pair<Key, T>, true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	enum CellState : char
	{
		EMPTY = 1,   //00000001
		DELETED = 2, //00000010
		OCCUPIED = 4,//00000100
	};
	
	static constexpr size_t initial_capacity{ 32 };
	Hash1 hash1_;
	Hash2 hash2_;
	std::vector<std::pair<Key, T>> data_;
	std::vector<CellState> states_;

	size_t double_hashing (Key key, size_t i)
	{
		auto h1 = hash1_(key) % data_.size();
		auto h2 = (1 + 2 * hash2_(key)) % data_.size();
		return (h1 + i * h2) % data_.size();
	}
	
public:
	open_addressing_map(size_t table_capacity = initial_capacity, Hash1 hash1 = Hash1(), Hash2 hash2 = Hash2()) :
		data_(table_capacity), states_(table_capacity, EMPTY), hash1_(hash1), hash2_(hash2) {}

	bool insert(const Key& key, const T& value)
	{
		for (size_t i = 0; i < data_.size(); ++i)
		{
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
	
	std::pair<Key, T> *search(const Key& key)
	{
		for (size_t i = 0; i < data_.size(); ++i)
		{
			size_t hash = double_hashing(key, i);
			if(states_[hash] == EMPTY)
			{
				return nullptr;
			}
			if (data_[hash].first == key)
			{
				return data_.data() + hash;
			}
		}
		return nullptr;
	}
	
	void erase(const Key& key)
	{
		for (size_t i = 0; i < data_.size(); ++i)
		{
			size_t hash = double_hashing(key, i);
			if (data_[hash].first == key)
			{
				states_[hash] = DELETED;
				return;
			}
		}
	}

	void print()
	{
		for (int i = 0; i < data_.size(); ++i)
		{
			if (states_[i] == OCCUPIED)
			{
				std::cout << data_[i].first;
				std::cout << " ";
				std::cout << data_[i].second;
				std::cout << std::endl;
			}
		}
	}

	iterator begin() {
		return iterator(data_, 0);
	}
	const_iterator begin() const {
		return const_iterator(data_, 0);
	}
	const_iterator cbegin() const {
		return const_iterator(data_, 0);
	}

	iterator end() {
		return iterator(data_, initial_capacity);
	}
	const_iterator end() const {
		return const_iterator(data_, initial_capacity);
	}
	const_iterator cend() const {
		return const_iterator(data_, initial_capacity);
	}
};

} /* namespace hash_tables */
