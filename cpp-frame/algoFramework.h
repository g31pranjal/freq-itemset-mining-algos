#ifndef ALGO

#define ALGO

#include "transactionDatabase.h"
#include <boost/dynamic_bitset.hpp>
#include <chrono>
#include <fstream>

class algoFramework {

	private :
		int minSupRelative;
		transactionDatabase * database;
		map<int, set<int> * > * verticalDB;
		int itemsetCount, algo, rec[4], N, M;
		ofstream writer; 

		void constructTIDSETS(vector<int> * equivalenceClassItems);
		void constructBITSETS(vector<int> * equivalenceClassItems);
		// void constructDIFFSETS(vector<int> * equivalenceClassItems);

		void processEquivalenceClassEclat(set<int> * prefixTidset, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<set<int> * > * equivalenceClassTidsets);
		set<int> * performINTERSECTION(set<int> * tidsetI, set<int> * tidsetJ);
		vector<set<int> * > * convertTIDSETStoDIFFSETS(set<int> * prefixTidset, vector<set<int> * > * equivalenceClassITidsets);
		set<int> * formParentDiffsUnionFromPrefixTidset(set<int> * prefixTidset);
		vector<boost::dynamic_bitset<> * > * convertTIDSETStoBITSETS(vector<set<int> * > * equivalenceClassITidsets);
		boost::dynamic_bitset<> * formPrefixBitsetFromPrefixTidset(set<int> * prefixTidset);
		
		void processEquivalenceClassViper(boost::dynamic_bitset<> * , int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<boost::dynamic_bitset<> * > * equivalenceClassBitsets);
		boost::dynamic_bitset<> * performAND(boost::dynamic_bitset<> * bitsetI, boost::dynamic_bitset<> * bitsetJ);
		vector<set<int> * > * convertBITSETStoTIDSETS(vector<boost::dynamic_bitset<> *> * equivalenceClassIBitsets);
		set<int> * formPrefixTidsetFromPrefixBitsets(boost::dynamic_bitset<> * prefixBitset);
		vector<set<int> * > * convertBITSETStoDIFFSETS(boost::dynamic_bitset<> * prefixBitset, vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets);
		set<int> * formParentDiffsUnionFromPrefixBitset(boost::dynamic_bitset<> * prefixBitset);
		
		void processEquivalenceClassDEclat(set<int> * parentDiffsUnion, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<set<int> * > * equivalenceClassDiffsets);
		set<int> * performDIFFERENCE(set<int> * diffsetI, set<int> * diffsetJ);
		vector<set<int> * > * convertDIFFSETStoTIDSETS(set<int> * parentDiffsUnion, vector<set<int> * > * equivalenceClassIDiffsets);
		set<int> * formPrefixTidsetFromParentDiffsUnion(set<int> * parentDiffsUnion);
		vector<boost::dynamic_bitset<> *> * convertDIFFSETStoBITSETS(set<int> * parentDiffsUnion, vector<set<int> * > * equivalenceClassIDiffsets);
		boost::dynamic_bitset<> * formPrefixBitsetFromParentDiffsUnion(set<int> * parentDiffsUnion);
		
		void printStats();
		void save(int * prefix, int prefixLength, int suffixItem, int support);

		// int INTSIZE = 32; //bits
		
	public :
		algoFramework();
		algoFramework(int algo);
		void runAlgo(char * outputFile, transactionDatabase * database, double minsupp);
		
		
};


#endif