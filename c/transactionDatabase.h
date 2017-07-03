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

	public :
		transactionDatabase();
		void loadFile(string path);
		void addTransaction(vector<string> itemsString);
		void printHorizontalDatabase();
		void printVerticalDatabase();
		int getN();
		int size();
		int getM();

		vector<vector<int> * > * getHorizontalDatabase();
		map<int, set<int> * > * getVerticalDatabase();
		set<int> * getItems();
		

};


#endif