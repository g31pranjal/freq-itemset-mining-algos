#ifndef UALGOFRAMEWORK

#define UALGOFRAMEWORK

#include "uTransactionDatabase.h"
#include "uTidset.h"
#include "uBitset.h"
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

		void constructTIDSETS(vector<int> * equivalenceClassItems);
		void constructBITSETS(vector<int> * equivalenceClassItems);
		//void constructDIFFSETS(vector<int> * equivalenceClassItems);
		
		void processEquivalenceClassEclat(int * prefix, int prefixLength, double prefixSupport, vector<int> * equivalenceClassItems, vector<uTidset * > * equivalenceClassTidsets);
		uTidset * performINTERSECTION(uTidset * tidsetI, uTidset * tidsetJ);

		void processEquivalenceClassViper(int * prefix, int prefixLength, double prefixSupport, vector<int> * equivalenceClassItems, vector<uBitset * > * equivalenceClassBitsets);
		uBitset * performAND(uBitset * bitsetI, uBitset * bitsetJ);

		void printStats();
		void save(int * prefix, int prefixLength, int suffixItem, double support);

	public:
		uAlgoFramework();
		uAlgoFramework(int algo);
		void runAlgo(char * outputFile, uTransactionDatabase * database, double minsupp);
	
};

#endif
