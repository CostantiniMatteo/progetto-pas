#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <algorithm>
#include <iostream>
#include <iterator>

template <typename T>
class SparseMatrix {
public:
	struct element {
		size_t x;
		size_t y;
		T value;
	};


private:
	struct node {
		node next;
		element value;

		node();
		node(const T &k, node *n = 0);
	};


	node head;
	size_t count;
	
public:

	typedef T value_type;

	SparseMatrix();

	~SparseMatrix();

	SparseMatrix(size_t n, size_t m, value_type def);

	SparseMatrix(const SparseMatrix &other);

	template <typename Q>
	SparseMatrix(const SparseMatrix<Q> &other);

	SparseMatrix &operator=(const SparseMatrix &other);

	size_t get_rows() const;

	size_t get_columns() const;

	size_t get_element_count() const;

	const value_type &get_dafault_value() const;

	void add(size_t x, size_t y, value_type value);

	value_type operator()(size_t x, size_t y);



private:
	

};


#endif