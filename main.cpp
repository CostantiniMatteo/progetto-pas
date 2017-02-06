#include "SparseMatrix.h"
#include <iostream>

using namespace std;

struct punto {
	double x;
	double y;
};

struct pair_string {
	string s1;
	string s2;
};

struct keep_if_equal {
	int init;
public:
	explicit keep_if_equal(int val) : init(val) {}
	bool operator()(int i) const {
		return (i == init);
	}
};





int main() {
	
	SparseMatrix<int> s(5,5,0);
	
	s.add(3,3,13);
	s.add(1,4,2);
	s.add(2,3,3);
	s.add(2,2,0);
  	s.add(1,2,4);
	s.add(1,3,2);
	s.add(1,1,16);
	s.add(2,4,17);
	s.add(2,0,15);
	s.add(0,4,25);
	s.add(0,4,0);

	return 0;	
}