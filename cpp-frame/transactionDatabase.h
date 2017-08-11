#ifndef TRANS_DB

#define TRANS_DB

#include "llSet.h"

#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class transactionDatabase {

	private :
		unordered_set<int> * items;
		vector<vector<int> * > * horizontalDB; 
		unordered_map<int, llSet * > * verticalDB; 
		int N, M;

		void dismantleHorizontalDatabase();
		void dismantleItems();
		void printHorizontalDatabase();

	public :
		transactionDatabase();
		~ transactionDatabase();
		void loadFile(string path);
		void addTransaction(vector<string> itemsString);
		void printVerticalDatabase();
		int getN();
		int getM();

		void dismantleVerticalDatabase();
		unordered_map<int, llSet * > * getVerticalDatabase();
		// vector<vector<int> * > * getHorizontalDatabase();
		// unordered_set<int> * getItems();
		
};

#endif