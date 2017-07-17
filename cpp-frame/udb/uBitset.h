#ifndef UBITSET_H
#define UBITSET_H 

#include <map>
#include <boost/dynamic_bitset.hpp>

using namespace std;

static double factors[] = {0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125, 0.00390625};
static int precision = 8;

class uBitset{
	private:
		double support;
		boost::dynamic_bitset<> * bitBucket[8];
		boost::dynamic_bitset<> * eligible;
	public:
		uBitset(int N);
		~ uBitset();
		double getSupport();
		boost::dynamic_bitset<> ** getBitBucket();
		boost::dynamic_bitset<> * getEligible();
		void insert(int tid, double probability);
		// void print();
};
#endif
