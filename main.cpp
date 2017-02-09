#include "SparseMatrix.h"
#include <cassert>
#include <iostream>
#include <math.h>

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

	explicit keep_if_equal(int val) : init(val) {}
	bool operator()(int i) const {
		return (i == init);
	}
};

struct mgt {
	double d;

	explicit mgt(double val) : d(val) {}
	bool operator()(punto_int p) const {
		return sqrt(p.x * p.x + p.y * p.y) > d;
	}
};

struct equal_strlen {
	bool operator()(const pair_string &p) const {
		return p.s1.size() == p.s2.size();
	}
};

struct even_len {
	bool operator()(const string &p) const {
		return p.size() % 2 == 0;
	}
};

bool even_len2(const string &p) {
	return p.size() % 2 == 0;
}


int main() {

	punto default_punto = {0,0};
	punto default_punto2 = {1.2,2.3};
	punto_int default_punto_int = {0,0};
	punto_int default_punto_int2 = {1,1};

	pair_string default_pair_string = {"a","a"};
	pair_string default_pair_string2 = {"ups","ups"};

	SparseMatrix<int> s_i(5,5,0);
	SparseMatrix<int> s_i2(2,4,3);
	SparseMatrix<double> s_d(5,5,1.1);
	SparseMatrix<string> s_s(5,5,"a");
	SparseMatrix<punto> s_p(15,5,default_punto);
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

	s_i2.add(0,0,2);
	s_i2.add(1,1,0);
	s_i2.add(1,0,3);
	s_i2.add(1,2,0);
	s_i2.add(0,2,0);

	s_d.add(0,0,2.4);
	s_d.add(1,2,2.3);
	s_d.add(4,4,5.4);

	s_s.add(0,0,"un");
	s_s.add(1,1,"due");
	s_s.add(2,2,"du");

	punto p1, p2, p3, p4;
	p1.x = 1.2; p1.y = 1.2; p2.x = 2.2; p2.y = 2.2;
	p3.x = 3.2; p3.y = 3.2; p4.x = 4.2; p4.y = 4.3;
	s_p.add(1,2,p1);
	s_p.add(10,2,p2);
	s_p.add(0,0,p3);
	s_p.add(14,4,p4);

	punto_int pi1, pi2, pi3, pi4;
	pi1.x = 0; pi1.y = 3; pi2.x = 3; pi2.y = 4;
	pi3.x = -3; pi3.y = 9; pi4.x = 7; pi4.y = 2;
	s_pi.add(1,3,pi1);
	s_pi.add(2,3,pi2);
	s_pi.add(4,2,pi3);
	s_pi.add(0,4,pi4);

	pair_string ps1, ps2, ps3, ps4;
	ps1.s1 = "una"; ps1.s2 = "stringa";
	ps2.s1 = "due"; ps2.s2 = "stringhe";
	ps3.s1 = "tre"; ps3.s2 = "stringhe";
	ps4.s1 = "quattro"; ps4.s2 = "stringhe";
	s_ps.add(0,0,ps1);
	s_ps.add(3,3,ps2);
	s_ps.add(2,2,ps3);
	s_ps.add(1,1,ps4);


	cout << "Stampa matrice di interi 1:" << endl;
	cout << "con iteratori..." << endl;
	cout << s_i << endl;
	cout << "con operator()" << endl;
	for(int i = 0; i < s_i.get_rows(); i++) {
		for(int j = 0; j < s_i.get_columns(); j++)
			cout << s_i(i,j) << " ";
		cout << endl;
	}

	cout << endl;
	cout << endl;

	cout << "Stampa matrice di interi 2:" << endl;
	cout << "con iteratori..." << endl;
	cout << s_i2 << endl;
	cout << "con operator()" << endl;
	for(int i = 0; i < s_i2.get_rows(); i++) {
		for(int j = 0; j < s_i2.get_columns(); j++)
			cout << s_i2(i,j) << " ";
		cout << endl;
	}

	cout << endl;
	cout << endl;


	cout << "Controllo count, rows, columns..." << endl;
	assert(s_i.get_rows() == 5);
	assert(s_i.get_columns() == 5);
	assert(s_i.get_default_value() == 0);
	assert(s_i.get_count() == 11);

	cout << endl;

	cout << "Modifica default value..." << endl;
	s_i.set_default_value(9);
	assert(s_i.get_default_value() == 9);
	for(int i = 0; i < s_i.get_rows(); i++) {
		for(int j = 0; j < s_i.get_columns(); j++)
			cout << s_i(i,j) << " ";
		cout << endl;
	}

	cout << endl;

	cout << "Controllo eval..." << endl;
	s_i.set_default_value(0);
	keep_if_equal kie(0);
	keep_if_equal kie2(2);
	assert(eval(s_i, kie) == 16);
	assert(eval(s_i, kie2) == 4);

	mgt mgt1(4);
	assert(eval(s_pi, mgt1) == 3);

	equal_strlen es;
	assert(eval(s_ps, es) == 21);

	even_len ev;
	assert(eval(s_s, ev) == 2);
	assert(eval(s_s, even_len2) == 2);

	cout << endl;

	cout << "Controllo operatore assegnamento..." << endl;
	s_i2 = s_i;
	assert(s_i.get_default_value() == 0);
	assert(s_i.get_rows() == 5);
	assert(s_i.get_columns() == 5);
	assert(s_i.get_count() == 11);

	cout << endl;

	cout << "Controllo copy constructor..." << endl;
	SparseMatrix<int> s_i3(s_i);
	assert(s_i3.get_default_value() == 0);
	assert(s_i3.get_rows() == 5);
	assert(s_i3.get_columns() == 5);
	assert(s_i3.get_count() == 11);
	s_i.add(0,1,3);
	assert(s_i3.get_count() == 11);

	cout << endl;

	cout << "Controllo costruttore con cast..." << endl;
	cout << "Double -> Int:" << endl;
	SparseMatrix<int> s_i4(s_d);
	cout << s_i4 << endl;
	cout << "Int -> Double:" << endl;
	SparseMatrix<double> s_d2(s_i);
	cout << s_d2 << endl;
	cout << endl;


	cout << "Controllo clear..." << endl;
	cout << "matrice di interi:" << endl;
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

	cout << endl;

	cout << "matrice di coppie di stringhe:" << endl;
	s_ps.clear();
	assert(s_ps.get_rows() == 5);
	assert(s_ps.get_columns() == 5);
	assert(s_ps.get_count() == 0);
	assert(s_ps.get_default_value() == default_pair_string);

	for(int i = 0; i < s_ps.get_rows(); i++) {
		for(int j = 0; j < s_ps.get_columns(); j++)
			cout << s_ps(i,j) << " ";
		cout << endl;
	}

	s_ps.add(0,0,ps1);
	s_ps.add(3,3,ps2);
	s_ps.add(2,2,ps3);
	s_ps.add(1,1,ps4);

	cout << endl;

	cout << "Stampa matrici di struct..." << endl;

	cout << "Con iteratori..." << endl;
	cout << "punti double:" << endl;
	cout << s_p << endl << endl;
	cout << "punti interi:" << endl;
	cout << s_pi << endl << endl;
	cout << "coppie di stringhe:" << endl;
	cout << s_ps << endl << endl;

	cout << "Con operator()..." << endl;
	cout << "punti double:" << endl;
	for(int i = 0; i < s_p.get_rows(); i++) {
		for(int j = 0; j < s_p.get_columns(); j++)
			cout << s_p(i,j) << " ";
		cout << endl;
	}

	cout << endl;
	cout << "punti interi:" << endl;
	for(int i = 0; i < s_pi.get_rows(); i++) {
		for(int j = 0; j < s_pi.get_columns(); j++)
			cout << s_pi(i,j) << " ";
		cout << endl;
	}

	cout << endl;
	cout << "coppie stringhe:" << endl;
	for(int i = 0; i < s_ps.get_rows(); i++) {
		for(int j = 0; j < s_ps.get_columns(); j++)
			cout << s_ps(i,j) << " ";
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