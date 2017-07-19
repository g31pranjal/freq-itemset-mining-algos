#include <unordered_set>
#include <boost/dynamic_bitset.hpp>
#include <boost/unordered_set.hpp>

using namespace std;

int main() {

	// unordered_set<int> * ptr;
	// ptr = new unordered_set<int>();

	boost::unordered_set<int> * ptr;
	ptr = new boost::unordered_set<int>();

	for(int i=0;i<1;i++) 
		ptr->insert(i);

}