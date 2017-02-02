#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <algorithm>
#include <iostream>
#include <iterator>
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

	
public:

	
	//SparseMatrix() = delete;

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

		while(current != 0 && current->value->x <= x && current->value->y <= y) {
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



private:
	

};


#endif