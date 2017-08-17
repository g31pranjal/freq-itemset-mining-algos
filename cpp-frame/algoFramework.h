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
		unordered_map<int, llSet * > * verticalDB;
		int itemsetCount, algo, rec[4], treeEdges[3][3], N, M;
		ofstream writer; 

		void constructTIDSETS(vector<int> * equivalenceClassItems);
		void constructBITSETS(vector<int> * equivalenceClassItems);
		void constructDIFFSETS(vector<int> * equivalenceClassItems);

		void processEquivalenceClassEclat(llSet * prefixTidset, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<llSet * > * equivalenceClassTidsets);
		llSet * performINTERSECTION(llSet * tidsetI, llSet * tidsetJ);
		vector<llSet * > * convertTIDSETStoDIFFSETS(llSet * prefixTidset, vector<llSet * > * equivalenceClassITidsets);
		llSet * formParentDiffsUnionFromPrefixTidset(llSet * prefixTidset);
		vector<boost::dynamic_bitset<> * > * convertTIDSETStoBITSETS(vector<llSet * > * equivalenceClassITidsets);
		boost::dynamic_bitset<> * formPrefixBitsetFromPrefixTidset(llSet * prefixTidset);
		
		void processEquivalenceClassViper(boost::dynamic_bitset<> * , int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<boost::dynamic_bitset<> * > * equivalenceClassBitsets);
		boost::dynamic_bitset<> * performAND(boost::dynamic_bitset<> * bitsetI, boost::dynamic_bitset<> * bitsetJ);
		vector<llSet * > * convertBITSETStoTIDSETS(vector<boost::dynamic_bitset<> *> * equivalenceClassIBitsets);
		llSet * formPrefixTidsetFromPrefixBitsets(boost::dynamic_bitset<> * prefixBitset);
		vector<llSet * > * convertBITSETStoDIFFSETS(boost::dynamic_bitset<> * prefixBitset, vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets);
		llSet * formParentDiffsUnionFromPrefixBitset(boost::dynamic_bitset<> * prefixBitset);
		
		void processEquivalenceClassDEclat(llSet * parentDiffsUnion, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<llSet * > * equivalenceClassDiffsets);
		llSet * performDIFFERENCE(llSet * diffsetI, llSet * diffsetJ);
		vector<llSet * > * convertDIFFSETStoTIDSETS(llSet * parentDiffsUnion, vector<llSet * > * equivalenceClassIDiffsets);
		llSet * formPrefixTidsetFromParentDiffsUnion(llSet * parentDiffsUnion);
		vector<boost::dynamic_bitset<> *> * convertDIFFSETStoBITSETS(llSet * parentDiffsUnion, vector<llSet * > * equivalenceClassIDiffsets);
		boost::dynamic_bitset<> * formPrefixBitsetFromParentDiffsUnion(llSet * parentDiffsUnion);
		
		void printStats();
		void save(int * prefix, int prefixLength, int suffixItem, int support);

		// int INTSIZE = 32; //bits
		
	public :
		algoFramework();
		algoFramework(int algo);
		void runAlgo(char * outputFile, transactionDatabase * database, double minsupp);
		
		
};


#endif