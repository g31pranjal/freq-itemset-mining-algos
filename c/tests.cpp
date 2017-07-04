#include <vector>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <chrono>
#include <boost/dynamic_bitset.hpp>

using namespace std;

volatile int sink;

int main() {
	
	for (auto size = 1ull; size < 1000000000ull; size *= 100) {
		
		chrono::_V2::system_clock::time_point start = chrono::system_clock::now();
		
		vector<int> v(size, 42);
		sink = accumulate(v.begin(), v.end(), 0u); // make sure it's a side effect
		
		chrono::_V2::system_clock::time_point  end = std::chrono::system_clock::now();

		chrono::duration<double> diff = end-start;

		cout << "Time to fill and iterate a vector of " << size << " ints : " << diff.count() << " s\n";

	}

}

// chrono::_V2::system_clock::time_point 
