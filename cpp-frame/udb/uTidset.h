#ifndef UTIDSET_H
#define UTIDSET_H 

#include <map>

using namespace std;

class uTidset{
	private:
		double support;
		map<int, double> tidAndProbMap;
	public:
		uTidset();
		~ uTidset();
		void insert(int tid, double probability);
		void print();
};
#endif
