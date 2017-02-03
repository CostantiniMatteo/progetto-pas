#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <cstddef>
#include <cassert>

template <typename T>
class SparseMatrix {
public:
	typedef T value_type;

	struct element {
		const size_t x;
		const size_t y;
		value_type value;

		element(size_t x, size_t y, value_type v) : x(x), y(y), value(v) {}
	};


private:
	struct node {
		element *value;
		node *next;

		node() : next(0) {}

		node(size_t x, size_t y, value_type val, node *n = 0) : value(new element(x, y, val)), next(n) {}
		
		~node() {
			delete value;
		}
	};


	node *head;
	size_t n_rows;
	size_t n_col;
	size_t count;
	value_type default_value;

	void _clear(node *n) {
		if(n == 0) {
			return;
		}

		node *tmp = n->next;
		delete n;
		n = 0;
		_clear(tmp);
	}

	SparseMatrix() {}

	
public:

	SparseMatrix(size_t n, size_t m, value_type def) : count(0),
		default_value(def), n_col(m), n_rows(n), head(0) {}


	SparseMatrix(const SparseMatrix &other) : count(0),
		default_value(other.default_value), n_col(other.n_col), n_rows(other.n_rows), head(0) {
		const node *tmp = other.head;

		try {
			while(tmp != 0) {
				add(tmp->value->x, tmp->value->y, tmp->value->value);
				tmp = tmp->next;
			}
		} catch(...) {
			clear();
			throw;
		}
	}

/*
	template <typename Q>
	SparseMatrix(const SparseMatrix<Q> &other);
*/

	~SparseMatrix() {
		clear();
	}


	SparseMatrix &operator=(const SparseMatrix &other) {
		if(this != &other) {
			SparseMatrix tmp(other);
			std::swap(head, tmp.head);
			std::swap(n_rows, tmp.n_rows);
			std::swap(n_col, tmp.n_col);
			std::swap(count, tmp.count);
			std::swap(default_value, tmp.default_value);
		}

		return *this;
	}


	size_t get_rows() const {
		return n_rows;
	}

	size_t get_columns() const {
		return n_col;
	}


	size_t get_count() const {
		return count;
	}


	const value_type &get_dafault_value() const {
		return default_value;
	}


	void add(size_t x, size_t y, const value_type &value) {
		assert(x < n_rows);
		assert(y < n_col);

		if(head == 0) {
			node* new_node = new node(x, y, value);
			head = new_node;
			count++;
			return;
		}

		node *current = head;
		node *prev = head;

		while(current != 0 && ((current->value->x < x) || (current->value->x <= x && current->value->y <= y))) {
			prev = current;
			current = current->next;
		}

		if(prev->value->x == x && prev->value->y == y) {
			prev->value->value = value;
			return;
		}

		node* new_node = new node(x, y, value);

		if(current == head) {
			new_node->next = head;
			head = new_node;
		} else {
			if(prev->value->x == x && prev->value->y == y) {
				prev->value->value = value;
			} else {
				new_node->next = prev->next;
				prev->next = new_node;
			}
		}

		count++;
	}


	const value_type operator()(size_t x, size_t y) const {
		assert(x < n_rows);
		assert(y < n_col);

		node *current = head;
		while(current != 0 && (current->value->x != x || current->value->y != y)) {
			current = current->next;
		}

		if(current == 0) {
			return default_value;
		} else {
			return current->value->value;
		}
	}

	//int eval() const();

	void clear() {
		_clear(head);
		count = 0;
	}

	void print_all() {
		node* current = head;

		while(current != 0) {
			std::cout << "[x:" << current->value->x << ", y:" << current->value->y << ", val:" << current->value->value << "]" << std::endl;
			current = current->next;
		}
	}




// ===================================================================
// ==ITERATOR========================================================
// ===================================================================

	class const_iterator;

	class iterator {
		node *n;

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef element                   value_type;
		typedef ptrdiff_t                 difference_type;
		typedef element*     		      pointer;
		typedef element&         	      reference;

	
		iterator() : n(0) {}
		
		iterator(const const_iterator &other) : n(other.n) {}

		const_iterator& operator=(const const_iterator &other) {
			n = other.n;
			return *this;
		}

		~iterator() {}

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() {
			return *(n->value);
		}

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() {
			return n->value;
		}
		
		// Operatore di iterazione post-incremento
		iterator operator++(int) {
			iterator tmp(*this);
			n = n->next;
			return tmp;
		}

		// Operatore di iterazione pre-incremento
		iterator& operator++() {
			n = n->next;
			return *this;
		}

		// Uguaglianza
		bool operator==(const iterator &other) {
			return (n == other.n);
		}
		
		// Diversita'
		bool operator!=(const iterator &other) {
			return (n != other.n);
		}

	private:
		friend class SparseMatrix;

		iterator(node *nn) : n(nn) {}
		
	}; // classe const_iterator
	
	// Ritorna l'iteratore all'inizio della sequenza dati
	iterator begin() {
		return iterator(head);
	}
	
	// Ritorna l'iteratore alla fine della sequenza dati
	iterator end() {
		return iterator(0);
	}
	
// ===================================================================
// ==ITERATOR=========================================================
// ===================================================================
	



// ===================================================================
// ==CONST ITERATOR===================================================
// ===================================================================

		
	class const_iterator {
		node *n;

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef element                   value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const element*            pointer;
		typedef const element&            reference;

	
		const_iterator() : n(0) {}
		
		const_iterator(const const_iterator &other) : n(other.n) {}

		const_iterator& operator=(const const_iterator &other) {
			n = other.n;
			return *this;
		}

		~const_iterator() {}

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return *(n->value);
		}

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() const {
			return n->value;
		}
		
		// Operatore di iterazione post-incremento
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			n = n->next;
			return tmp;
		}

		// Operatore di iterazione pre-incremento
		const_iterator& operator++() {
			n = n->next;
			return *this;
		}

		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (n == other.n);
		}
		
		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return (n != other.n);
		}

	private:
		friend class SparseMatrix;

		const_iterator(node *nn) : n(nn) {}
		
	}; // classe const_iterator
	
	// Ritorna l'iteratore all'inizio della sequenza dati
	const_iterator begin() const {
		return const_iterator(head);
	}
	
	// Ritorna l'iteratore alla fine della sequenza dati
	const_iterator end() const {
		return const_iterator(0);
	}
	
// ===================================================================
// ==CONST ITERATOR===================================================
// ===================================================================
	
};


#endif