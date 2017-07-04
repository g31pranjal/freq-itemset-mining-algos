#include <vector>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <boost/dynamic_bitset.hpp>

using namespace std;


int main() {

	boost::dynamic_bitset<> * x = new boost::dynamic_bitset<>(10);
	boost::dynamic_bitset<> * y = new boost::dynamic_bitset<>(10);
	
	(*x)[0] = 1;
	(*x)[3] = 1;
	(*x)[4] = 1;
	(*x)[6] = 1;
	(*x)[9] = 1;

	int j=0;

	for(int i = x->find_first(); i >= 0; i = x->find_next(i)) {
		cout << (j++) << " : " << i << endl;
	}

	
	boost::dynamic_bitset<> * z = new boost::dynamic_bitset<>((*x)&(*y));

	delete x;
	delete y;
	delete z;

	string s;
	to_string(*x,s);

	cout << s;

	// bitset<10> * a = new bitset<10>(string("1010101010") );

	// bitset<10> * b = new bitset<10>(string("0101010101") );

	// bitset<10> * s = new bitset<10>( (*a)&(*b) );

	// cout << s->to_string();

}