#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "ordered_list.h"
#include "sparse_matrix.h"


using namespace std;

// funtore generico che funziona su tutti i primitivi


/*
struct compare_int {
	int operator()(const int &a, const int &b) const {
		return a - b;
	}
};
*/

//int compare_int(const int a, const int b) { return a - b; }

struct compare_string_length {
	int operator()(const string &a, const string &b) const {
		return a.size() - b.size();
	}
};


int main(int argc, char* argv[]) {
	
	ordered_list<int> a;

	a.insert(56);
	a.insert(90);
	a.insert(60);
	a.insert(60);
	a.insert(5);

	a.print();

	ordered_list<string, compare_string_length> b;

	b.insert("Cacca");
	b.insert("ciao");
	b.insert("Cacca");
	b.insert("cacca");

	cout << a.find(5) << endl;
	cout << b.find("3") << b.find("Ciao") << endl;

	cout << b << endl;

	ordered_list<int> c(a.begin(), a.end());

	cout << c << endl;

	return 0;
}
