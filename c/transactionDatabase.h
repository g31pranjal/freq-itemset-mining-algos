#ifndef TRANS_DB

#define TRANS_DB

#include <set>
#include <vector>
#include <map>
#include <string>

using namespace std;

class transactionDatabase {

	private :
		set<int> * items;
		vector<vector<int> * > * horizontalDB; 
		map<int, set<int> * > * verticalDB; 
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
		map<int, set<int> * > * getVerticalDatabase();
		// vector<vector<int> * > * getHorizontalDatabase();
		// set<int> * getItems();
		
};

#endif