#ifndef UALGOFRAMEWORK
#define UALGOFRAMEWORK

#include "UTransactionDatabase.h"
#include <boost/dynamic_bitset.hpp>
#include <chrono>
#include <fstream>

class uAlgoFramework
{
	private:
		int minSupRelative;
		UTransactionDatabase * database;
		map<int, set<TidAndProb *> *> * verticalDB;
		int itemsetCount, algo, rec[4], N, M;
		ofstream writer;

		//void constructTIDSETS(vector<int> * equivalenceClassItems);
		//void constructBITSETS(vector<int> * equivalenceClassItems);
		//void constructDIFFSETS(vector<int> * equivalenceClassItems);
		void processEquivalenceClassEclat(set<int> * prefixTidset, int * prefix, int prefixLength, double prefixSupport, vector<TidAndProb *> * equivalenceClassItems, vector<set<int> * > * equivalenceClassTidsets);

	public:
		uAlgoFramework();
		uAlgoFramework(int algo);
		void runAlgo(char * outputFile, UTransactionDatabase * database, double minsupp);
	
};

#endif