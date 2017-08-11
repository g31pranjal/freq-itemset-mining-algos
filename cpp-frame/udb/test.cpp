#include <unordered_map>
#include <unordered_set>

#include "uTidset.h"
#include "uBitset.h"

using namespace std;

int main() {

	// uTidset * pt;

	// pt = new uTidset();

	// for(int i=0;i<1000000;i++) {
	// 	pt->insert(i, 0.0001*(double)i);
	// }

	unordered_set<int> * pt;
	pt = new unordered_set<int>();

	for(int i=0;i<100000;i++) {
		pt->insert(i);
	}





}