#include "SparseMatrix.h"
#include <iostream>

using namespace std;

void test_sm() {
	SparseMatrix<int> m(10,10,0);
	SparseMatrix<int> m2(5,5,1);
	
	m.add(1,2,2);
	m.add(3,2,4);
	m.add(0,0,1);
	m.add(0,0,7);
	m.add(1,2,3);
	m.add(2,2,9);

	m2.add(0,0,3);
	

	for(int i = 0; i < m.get_rows(); i++) {
		for(int j = 0; j < m.get_columns(); j++) {
			//cout << m(i,j) << " ";
			m(i,j);
		}
		//cout << endl;
	}
/*
	cout << endl;

	m.print_all();
	cout << m.get_count() << endl;;
	cout << (0 == m.get_dafault_value() ? "true" : "false") << endl;
	cout << (1 == m.get_dafault_value() ? "true" : "false") << endl;
	cout << endl;
	cout << endl;
*/


	m = m2;

	for(int i = 0; i < m.get_rows(); i++) {
		for(int j = 0; j < m.get_columns(); j++) {
			//cout << m(i,j) << " ";
			m(i,j);
		}
		//cout << endl;
	}
/*
	cout << endl;

	m.print_all();
	cout << m.get_count() << endl;;
	cout << (1 == m.get_dafault_value() ? "true" : "false") << endl;
	cout << (0 == m.get_dafault_value() ? "true" : "false") << endl;
*/
}

int main() {
	
	test_sm();

	return 0;	
}