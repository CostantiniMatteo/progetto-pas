#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include "ordered_list.h"

template <typename T>
struct element {
    const int i;
    const int j;
    T value;
    
    element(int i, int j, T value);
};

struct compare_element {
	int operator()(const element &a, const element &b) const {
		if(a.i != b.i)
		    return a.i - b.i;
		else
		    return a.j - b.j;
	}
};

template <typename T>
class sparse_matrix {
    
private:
    ordered_list<element, compare_element> values;

};

#endif