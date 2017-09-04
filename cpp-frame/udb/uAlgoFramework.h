#ifndef UALGOFRAMEWORK

#define UALGOFRAMEWORK

#include "uTransactionDatabase.h"
#include "ullSet.h"
#include "uBitset.h"
#include <boost/dynamic_bitset.hpp>
#include <chrono>
#include <fstream>

class uAlgoFramework {
	private:
		double minSupRelative;
		uTransactionDatabase * database;
		map<int, ullSet *> * verticalDB;
		int itemsetCount, algo, rec[4], N, M;
		ofstream writer;

		void constructTIDSETS(vector<int> * equivalenceClassItems);
		void constructBITSETS(vector<int> * equivalenceClassItems);
		
		void processEquivalenceClassEclat(int * prefix, int prefixLength, double prefixSupport, vector<int> * equivalenceClassItems, vector<ullSet * > * equivalenceClassTidsets, ullSet * prefixSupportSet);
		ullSet * performINTERSECTION(ullSet * tidsetI, ullSet * tidsetJ, ullSet * prefixSupportSet);
		vector<uBitset * > * convertTIDSETStoBITSETS(vector<ullSet * > * equivalenceClassITidsets);

		void processEquivalenceClassViper(int * prefix, int prefixLength, double prefixSupport, vector<int> * equivalenceClassItems, vector<uBitset * > * equivalenceClassBitsets, uBitset * prefixBitsets);
		uBitset * performAND(uBitset * bitsetI, uBitset * bitsetJ, uBitset * prefixBitsets);
		vector<ullSet * > * convertBITSETStoTIDSETS(vector<uBitset *> * equivalenceClassIBitsets);

		void printStats();
		void save(int * prefix, int prefixLength, int suffixItem, double support);

	public:
		uAlgoFramework();
		uAlgoFramework(int algo);
		void runAlgo(char * outputFile, uTransactionDatabase * database, double minsupp);
	
};

#endif
