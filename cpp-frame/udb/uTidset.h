#ifndef UTIDSET_H
#define UTIDSET_H 

#include <unordered_map>

using namespace std;

class uTidset{
	private:
		double support;
		unordered_map<int, double> * tidAndProbMap;
	public:
		uTidset();
		~ uTidset();
		double getSupport();
		unordered_map<int, double> * getMap();
		void insert(int tid, double probability);
		void print();
};
#endif
