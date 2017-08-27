#ifndef UBITSET_H
#define UBITSET_H 

#include <map>
#include <boost/dynamic_bitset.hpp>

using namespace std;

static int precision = 16;

class uBitset{
	private:
		double support;
		int num;
		boost::dynamic_bitset<> ** bitBucket;
		boost::dynamic_bitset<> * eligible;
	public:
		uBitset(int N);
		~ uBitset();
		double getSupport();
		int size();
		boost::dynamic_bitset<> ** getBitBucket();
		boost::dynamic_bitset<> * getEligible();
		double getProbability(int tid);
		void insert(int tid, double probability);
		// void print();
};
#endif
