#include "SparseMatrix.h"
#include <iostream>
#include <cassert>

using namespace std;

struct punto {
	double x;
	double y;

	friend std::ostream& operator<<(std::ostream& os, const punto& p) {
		os << "{x : " << p.x
			<< ", y : " << p.y << "}";

		return os;
	}

	bool operator==(const punto &other) const {
		return this->x == other.x && this->y == other.y;
	}

	bool operator!=(const punto &other) const {
		return !(*this == other);
	}
	
};

struct punto_int {
	int x;
	int y;

	friend std::ostream& operator<<(std::ostream& os, const punto_int& p) {
		os << "{x : " << p.x
			<< ", y : " << p.y << "}";

		return os;
	}

	bool operator==(const punto_int &other) const {
		return this->x == other.x && this->y == other.y;
	}

	bool operator!=(const punto_int &other) const {
		return !(*this == other);
	}
};

struct pair_string {
	string s1;
	string s2;

	friend std::ostream& operator<<(std::ostream& os, const pair_string& s) {
		os << "{s1 : " << s.s1
			<< ", s2 : " << s.s2 << "}";

		return os;
	}

	bool operator==(const pair_string &other) const {
		return this->s1 == other.s1 && this->s2 == other.s2;
	}

	bool operator!=(const pair_string &other) const {
		return !(*this == other);
	}
};

struct keep_if_equal {
	int init;
public:
	explicit keep_if_equal(int val) : init(val) {}
	bool operator()(int i) const {
		return (i == init);
	}
};


/*
	Metodi da testare:
	clear
	costruttore standard
	copy constructor
	costruttore con cast
	assegnamento

	rows comuln e count
	default value (get e set)

	add
	operator()

	eval

	operator<<
*/


int main() {

	punto default_punto = {0,0};
	punto default_punto2 = {1.2,2.3};
	punto_int default_punto_int = {0,0};
	punto_int default_punto_int2 = {1,1};
	pair_string default_pair_string = {"",""};
	pair_string default_pair_string2 = {"cacca","cacca"};

	SparseMatrix<int> s_i(5,5,0);
	SparseMatrix<int> s_i2(2,2,3);
	SparseMatrix<double> s_d(5,5,10);
	SparseMatrix<string> s_s(5,5,"");
	SparseMatrix<punto> s_p(5,5,default_punto);
	SparseMatrix<punto_int> s_pi(5,5,default_punto_int);
	SparseMatrix<pair_string> s_ps(5,5,default_pair_string);

	
	s_i.add(3,3,1);
	s_i.add(1,4,2);
	s_i.add(2,3,3);
	s_i.add(2,2,0);
  	s_i.add(1,2,4);
	s_i.add(1,3,2);
	s_i.add(1,1,1);
	s_i.add(2,4,2);
	s_i.add(4,4,7);
	s_i.add(4,4,8);
	s_i.add(1,1,2);
	s_i.add(2,0,1);
	s_i.add(0,4,4);
	s_i.add(0,4,0);



	cout << "Stampa matrice di interi:" << endl;
	cout << "con iteratori..." << endl;
	cout << s_i << endl;
	cout << "con operator()" << endl;
	for(int i = 0; i < s_i.get_rows(); i++) {
		for(int j = 0; j < s_i.get_columns(); j++)
			cout << s_i(i,j) << " ";
		cout << endl;
	}

	cout << "Controllo count, rows, columns..." << endl;
	assert(s_i.get_rows() == 5);
	assert(s_i.get_columns() == 5);
	assert(s_i.get_default_value() == 0);
	assert(s_i.get_count() == 11);

	cout << "Modifica default value..." << endl;
	s_i.set_default_value(9);
	for(int i = 0; i < s_i.get_rows(); i++) {
		for(int j = 0; j < s_i.get_columns(); j++)
			cout << s_i(i,j) << " ";
		cout << endl;
	}

	cout << "Controllo eval..." << endl;
	s_i.set_default_value(0);
	keep_if_equal kie(0);
	keep_if_equal kie2(2);
	assert(eval(s_i, kie) == 16);
	assert(eval(s_i, kie2) == 4);

	cout << "Controllo operatore assegnamento..." << endl;
	s_i2 = s_i;
	assert(s_i.get_default_value() == 0);
	assert(s_i.get_rows() == 5);
	assert(s_i.get_columns() == 5);
	assert(s_i.get_count() == 11);

	cout << "Controllo copy constructor..." << endl;
	SparseMatrix<int> s_i3(s_i);
	assert(s_i3.get_default_value() == 0);
	assert(s_i3.get_rows() == 5);
	assert(s_i3.get_columns() == 5);
	assert(s_i3.get_count() == 11);
	s_i.add(0,1,3);
	assert(s_i3.get_count() == 11);

	cout << "Controllo clear..." << endl;
	s_i.clear();
	assert(s_i.get_rows() == 5);
	assert(s_i.get_columns() == 5);
	assert(s_i.get_default_value() == 0);
	assert(s_i.get_count() == 0);

	for(int i = 0; i < s_i.get_rows(); i++) {
		for(int j = 0; j < s_i.get_columns(); j++)
			cout << s_i(i,j) << " ";
		cout << endl;
	}

	cout << "Stampa matrici di struct..." << endl;

	cout << "Con iteratori..." << endl;
	cout << s_p << endl << endl;
	cout << s_pi << endl << endl;
	cout << s_ps << endl << endl;

	cout << "Con operator()..." << endl;
	for(int i = 0; i < s_p.get_rows(); i++) {
		for(int j = 0; j < s_p.get_columns(); j++)
			cout << s_p(i,j) << " ";
		cout << endl;
	}

	cout << endl;

	for(int i = 0; i < s_pi.get_rows(); i++) {
		for(int j = 0; j < s_pi.get_columns(); j++)
			cout << s_pi(i,j) << " ";
		cout << endl;
	}


	cout << "Controllo valori di default..." << endl;
	assert(s_p.get_default_value() == default_punto);
	s_p.set_default_value(default_punto2);
	assert(s_p.get_default_value() == default_punto2);
	assert(s_p.get_default_value() != default_punto);

	assert(s_pi.get_default_value() == default_punto_int);
	s_pi.set_default_value(default_punto_int2);
	assert(s_pi.get_default_value() == default_punto_int2);
	assert(s_pi.get_default_value() != default_punto_int);

	assert(s_ps.get_default_value() == default_pair_string);
	s_ps.set_default_value(default_pair_string2);
	assert(s_ps.get_default_value() == default_pair_string2);
	assert(s_ps.get_default_value() != default_pair_string);

	return 0;	
}