#pragma once
#include <algorithm>

template <typename K, typename V>
class d_linked_list
{
	using T = std::pair<K, V>;
	class node
	{
	public:
		T value;
		node* prev;
		node* next;

		node(T value, node* prev = nullptr, node* next = nullptr)
			: value{ std::move(value) }, prev{ prev }, next{ next }{}
		node(const node&) = delete;
		void operator=(const node&) = delete;

		friend void swap(node& a, node& b) {
			using std::swap;
			swap(a.value, b.value);
			swap(a.prev, b.prev);
			swap(a.next, b.next);
		}
	};
public:
	d_linked_list() = default;
	d_linked_list(const d_linked_list<K, V>&);
	d_linked_list(const d_linked_list<K, V>&&);
	d_linked_list& operator=(d_linked_list<K, V>);
	~d_linked_list();
	T const& get(std::size_t pos) const;
	T& get(std::size_t pos);
	T* find(const K& key) const;
	void insert(T value, std::size_t pos);
	//T remove(std::size_t pos);
	T remove(const K& key);

	template<typename U, typename W>
	friend void swap(d_linked_list<U, W>&, d_linked_list<U, W>&);
	size_t n_elements_ = 0;
private:
	node head_ = { T{}, nullptr, &tail_ };
	node tail_ = { T{}, &head_, nullptr };
	//size_t n_elements_ = 0;
};

template<typename K, typename V>
d_linked_list<K, V>::d_linked_list(const d_linked_list<K, V>& other)
{
	// work from back to front, so we always insert at position 0
	// (which is the cheapest)
	for (auto i = other.n_elements_; i > 0; --i) {
		insert(other.get(i - 1), 0);
	}
}

template<typename K, typename V>
d_linked_list<K, V>::d_linked_list(const d_linked_list<K, V>&& other)
{
	swap(*this, other);
}

template<typename K, typename V>
d_linked_list<K, V>& d_linked_list<K, V>::operator=(d_linked_list<K, V> other)
{
	swap(*this, other);
	return *this;
}

template<typename K, typename V>
void swap(d_linked_list<K, V>& a, d_linked_list<K, V>& b)
{
	using std::swap;
	swap(a.head_, b.head_);
	swap(a.tail_, b.tail_);
	swap(a.n_elements_, b.n_elements_);
}

template <typename K, typename V>
d_linked_list<K, V>::~d_linked_list()
{
	for (auto p = head_.next; p != &tail_; ) {
		auto next = p->next;
		delete p;
		p = next;
	}
}

template <typename K, typename V>
typename d_linked_list<K, V>::T const& d_linked_list<K, V>::get(std::size_t pos) const
{
	auto p = head_.next;
	while (pos--)
		p = p->next;
	return p->value;
}

template<typename K, typename V>
typename d_linked_list<K, V>::T& d_linked_list<K, V>::get(std::size_t pos)
{
	auto p = head_.next;
	while (pos--)
		p = p->next;
	return p->value;
}

template <typename K, typename V>
typename d_linked_list<K, V>::T*  d_linked_list<K, V>::find(const K& key) const
{
	auto p = head_.next;
	while (key != p->value.first)
	{
		if (p->next == nullptr)
		{
			//std::cout << "Not found." << std::endl;
			return p->value;
		}
		p = p->next;
	}
	return &p->value;
}

template<typename K, typename V>
void d_linked_list<K, V>::insert(T value, std::size_t pos)
{
	auto p = &head_;
	while (pos--)
		p = p->next;
	auto next = p->next;
	next->prev = p->next = new node(std::move(value), p, next);
	++n_elements_;
}

//template<typename K, typename V>
//typename d_linked_list<K, V>::T d_linked_list<K, V>::remove(std::size_t pos)
//{
//	auto p = head_.next;
//	while (pos--)
//		p = p->next;
//	p->prev->next = p->next;
//	p->next->prev = p->prev;
//	auto value = std::move(p->value);
//	delete p;
//	--n_elements_;
//	return value;
//}

template<typename K, typename V>
typename d_linked_list<K, V>::T d_linked_list<K, V>::remove(const K& key)
{
	auto p = head_.next;
	while (key != p->value.first)
	{
		if (p->next == nullptr)
		{
			std::cout << "Not found." << std::endl;
			return p->value;
		}
		p = p->next;
	}
	p->prev->next = p->next;
	p->next->prev = p->prev;
	auto value = std::move(p->value);
	delete p;
	--n_elements_;
	//std::cout << "Deleted " << value.first << ' ' << value.second << std::endl;
	return value;
}