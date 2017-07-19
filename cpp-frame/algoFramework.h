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
		unordered_map<int, unordered_set<int> * > * verticalDB;
		int itemsetCount, algo, rec[4], treeEdges[3][3], N, M;
		ofstream writer; 

		void constructTIDSETS(vector<int> * equivalenceClassItems);
		void constructBITSETS(vector<int> * equivalenceClassItems);
		void constructDIFFSETS(vector<int> * equivalenceClassItems);

		void processEquivalenceClassEclat(unordered_set<int> * prefixTidset, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<unordered_set<int> * > * equivalenceClassTidsets);
		unordered_set<int> * performINTERSECTION(unordered_set<int> * tidsetI, unordered_set<int> * tidsetJ);
		vector<unordered_set<int> * > * convertTIDSETStoDIFFSETS(unordered_set<int> * prefixTidset, vector<unordered_set<int> * > * equivalenceClassITidsets);
		unordered_set<int> * formParentDiffsUnionFromPrefixTidset(unordered_set<int> * prefixTidset);
		vector<boost::dynamic_bitset<> * > * convertTIDSETStoBITSETS(vector<unordered_set<int> * > * equivalenceClassITidsets);
		boost::dynamic_bitset<> * formPrefixBitsetFromPrefixTidset(unordered_set<int> * prefixTidset);
		
		void processEquivalenceClassViper(boost::dynamic_bitset<> * , int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<boost::dynamic_bitset<> * > * equivalenceClassBitsets);
		boost::dynamic_bitset<> * performAND(boost::dynamic_bitset<> * bitsetI, boost::dynamic_bitset<> * bitsetJ);
		vector<unordered_set<int> * > * convertBITSETStoTIDSETS(vector<boost::dynamic_bitset<> *> * equivalenceClassIBitsets);
		unordered_set<int> * formPrefixTidsetFromPrefixBitsets(boost::dynamic_bitset<> * prefixBitset);
		vector<unordered_set<int> * > * convertBITSETStoDIFFSETS(boost::dynamic_bitset<> * prefixBitset, vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets);
		unordered_set<int> * formParentDiffsUnionFromPrefixBitset(boost::dynamic_bitset<> * prefixBitset);
		
		void processEquivalenceClassDEclat(unordered_set<int> * parentDiffsUnion, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<unordered_set<int> * > * equivalenceClassDiffsets);
		unordered_set<int> * performDIFFERENCE(unordered_set<int> * diffsetI, unordered_set<int> * diffsetJ);
		vector<unordered_set<int> * > * convertDIFFSETStoTIDSETS(unordered_set<int> * parentDiffsUnion, vector<unordered_set<int> * > * equivalenceClassIDiffsets);
		unordered_set<int> * formPrefixTidsetFromParentDiffsUnion(unordered_set<int> * parentDiffsUnion);
		vector<boost::dynamic_bitset<> *> * convertDIFFSETStoBITSETS(unordered_set<int> * parentDiffsUnion, vector<unordered_set<int> * > * equivalenceClassIDiffsets);
		boost::dynamic_bitset<> * formPrefixBitsetFromParentDiffsUnion(unordered_set<int> * parentDiffsUnion);
		
		void printStats();
		void save(int * prefix, int prefixLength, int suffixItem, int support);

		// int INTSIZE = 32; //bits
		
	public :
		algoFramework();
		algoFramework(int algo);
		void runAlgo(char * outputFile, transactionDatabase * database, double minsupp);
		
		
};


#endif