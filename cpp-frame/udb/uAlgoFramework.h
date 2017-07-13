#ifndef UALGOFRAMEWORK
#define UALGOFRAMEWORK

#include "uTransactionDatabase.h"
#include "uTidset.h"
#include <boost/dynamic_bitset.hpp>
#include <chrono>
#include <fstream>

class uAlgoFramework
{
	private:
		double minSupRelative;
		uTransactionDatabase * database;
		map<int, uTidset *> * verticalDB;
		int itemsetCount, algo, rec[4], N, M;
		ofstream writer;

		//void constructTIDSETS(vector<int> * equivalenceClassItems);
		//void constructBITSETS(vector<int> * equivalenceClassItems);
		//void constructDIFFSETS(vector<int> * equivalenceClassItems);
		void processEquivalenceClassEclat(set<int> * prefixTidset, int * prefix, int prefixLength, double prefixSupport, vector<tidAndProb *> * equivalenceClassItems, vector<set<int> * > * equivalenceClassTidsets);

	public:
		uAlgoFramework();
		uAlgoFramework(int algo);
		void runAlgo(char * outputFile, uTransactionDatabase * database, double minsupp);
	
};

#endif
