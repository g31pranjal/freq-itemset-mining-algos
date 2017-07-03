#ifndef ALGO

#define ALGO

#include "transactionDatabase.h"

class algoFramework {

	private :
		int minSupRelative;
		transactionDatabase * database;
		map<int, set<int> * > * verticalDB;
		int itemsetCount, algo, rec[4], N, M;

		void constructTIDSETS(vector<int> * equivalenceClassItems);
		void constructBITSETS(vector<int> * equivalenceClassItems);
		void constructDIFFSETS(vector<int> * equivalenceClassItems);

		void processEquivalenceClassEclat(set<int> * prefixTidset, int prefix[], int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<set<int> * > * equivalenceClassTidsets);
		set<int> * performINTERSECTION(set<int> * tidsetI, set<int> * tidsetJ);
		vector<set<int> * > * convertTIDSETStoDIFFSETS(set<int> * prefixTidset, vector<set<int> * > * equivalenceClassITidsets);
		set<int> * formParentDiffsUnionFromPrefixTidset(set<int> * prefixTidset);
		// vector<BitsetSupport> convertTIDSETStoBITSETS(vector<set<int>> equivalenceClassITidsets);
		// BitsetSupport formPrefixBitsetFromPrefixTidset(set<int> prefixTidset);
		
		// void processEquivalenceClassViper(BitsetSupport prefixBitset, int[] prefix, int prefixLength, int prefixSupport, vector<int> equivalenceClassItems, vector<BitsetSupport> equivalenceClassBitsets);
		// BitsetSupport performAND(BitsetSupport bitsetI, BitsetSupport bitsetJ)
		// vector<set<int>> convertBITSETStoTIDSETS(vector<BitsetSupport> equivalenceClassIBitsets);
		// set<int> formPrefixTidsetFromPrefixBitsets(BitsetSupport prefixBitset);
		// vector<set<int>> convertBITSETStoDIFFSETS(BitsetSupport prefixBitset, vector<BitsetSupport> equivalenceClassIBitsets);
		// set<int> formParentDiffsUnionFromPrefixBitset(BitsetSupport prefixBitset);
		
		void processEquivalenceClassDEclat(set<int> * parentDiffsUnion, int prefix[], int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<set<int> * > * equivalenceClassDiffsets);
		set<int> * performDIFFERENCE(set<int> * diffsetI, set<int> * diffsetJ);
		vector<set<int> * > * convertDIFFSETStoTIDSETS(set<int> * parentDiffsUnion, vector<set<int> * > * equivalenceClassIDiffsets);
		set<int> * formPrefixTidsetFromParentDiffsUnion(set<int> * parentDiffsUnion);
		// vector<BitsetSupport> convertDIFFSETStoBITSETS(set<int> parentDiffsUnion, vector<set<int>> equivalenceClassIDiffsets);
		// BitsetSupport formPrefixBitsetFromParentDiffsUnion(set<int> parentDiffsUnion);
		
		// void printStats();
		// void save(int[] prefix, int prefixLength, int suffixItem, int support);

		// BufferedWriter writer = null; 
		// Itemsets frequentItemsets;
		// int INTSIZE = 32; //bits
		//long startTime;
		//long endTime;
		
	public :
		algoFramework();
		algoFramework(int algo);
		void runAlgo(string outputFile, transactionDatabase * database, double minsupp);
		
		
};


#endif