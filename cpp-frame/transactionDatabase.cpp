#include "transactionDatabase.h"
#include "llSet.h"

#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

vector<string> stringSplit(string str, string sep){
	char * cstr = const_cast<char *>(str.c_str());
	char * current;
	vector<string> arr;
	current = strtok(cstr, sep.c_str());
	while(current != NULL){
		arr.push_back(current);
		current=strtok(NULL,sep.c_str());
	}
	return arr;
}

transactionDatabase::transactionDatabase() {

	items = new unordered_set<int>();
	horizontalDB = new vector<vector<int> * >(); 
	verticalDB = new unordered_map<int, llSet * >(); 
}

transactionDatabase::~ transactionDatabase() {
	this->dismantleItems();
	this->dismantleHorizontalDatabase();
	this->dismantleVerticalDatabase();
}

void transactionDatabase::loadFile(string path){
		
	string thisLine; 
	ifstream infile(path.c_str());

	while(getline(infile, thisLine)) {
		if(thisLine.length() != 0 && thisLine[0] != '#' && thisLine[0] != '%' && thisLine[0] != '@') {
			transactionDatabase::addTransaction(stringSplit(thisLine, " "));
		} 
	}

	N = this->horizontalDB->size();

	// convert to vertical datastructure.	
	
	vector<int> * txn;
	llSet * st;

	for (int i = 0; i < this->getN(); i++) {
		txn = horizontalDB->at(i);
		for (int j=0;j<txn->size();j++) {
			if(verticalDB->find(txn->at(j)) == verticalDB->end() ){
				st = new llSet();
				verticalDB->insert(make_pair(txn->at(j), st));
			}
			else{
				st = verticalDB->at(txn->at(j));
			}
			st->addElement(i); 
		}
	}

	M = this->items->size();

	// dismantle unused structures

	this->dismantleHorizontalDatabase();
	this->dismantleItems();
}


void transactionDatabase::addTransaction(vector<string> itemsString) {
	vector<int> * itemset = new vector<int>();
		
	for(vector<string>::iterator i = itemsString.begin(); i != itemsString.end(); i++) {
		int item = stoi(*i);
		itemset->push_back(item); 
		items->insert(item);
	}
	horizontalDB->push_back(itemset);
}

int transactionDatabase::getN() {
	return this->N;
}

// vector<vector<int> * > * transactionDatabase::getHorizontalDatabase() {
// 	return horizontalDB;
// }

unordered_map<int, llSet * > * transactionDatabase::getVerticalDatabase() {
	return this->verticalDB;
}
	
// unordered_set<int> * transactionDatabase::getItems() {
// 	return items;
// }
	
int transactionDatabase::getM() {
	return this->M;
}

void transactionDatabase::printHorizontalDatabase() {
	cout << "\n... Transaction Database :: (horizontal)\n";
	int count = 0; 
	
	for (int i=0;i<horizontalDB->size();i++) { 
		cout << (count++) << " : ";
		for(int j=0;j<horizontalDB->at(i)->size();j++) {
			cout << horizontalDB->at(i)->at(j) << " ";	
		}
		cout << endl;
	}

	cout << "# transactions (n) " << this->getN() << ", # items (m) : " << this->getM() << endl;
}

void transactionDatabase::printVerticalDatabase() {
	llSet * st;
	cout << "\n... Transaction Database :: (vertical)\n";
	for(unordered_map<int, llSet *>::iterator i = verticalDB->begin() ;i != verticalDB->end();i++) {
		cout << i->first << " : ";
		st = i->second;
		for(llSet_element * ele = st->getFirst();ele != NULL; ele = ele->getNext() ) 
			cout << ele->getValue() << " ";
		cout << endl;
	}
	
	cout << "# transactions (n) " << this->getN() << ", # items (m) : " << this->getM() << endl;
}

void transactionDatabase::dismantleHorizontalDatabase() {
	if(horizontalDB != NULL) {
		for(int i=0;i<horizontalDB->size();i++) {
			if(horizontalDB->at(i) != NULL)
				delete horizontalDB->at(i);
		}
		delete horizontalDB;
		horizontalDB = NULL;
	}
}

void transactionDatabase::dismantleVerticalDatabase() {
	if(verticalDB != NULL) {
		for(unordered_map<int, llSet * >::iterator i = verticalDB->begin(); i != verticalDB->end(); i++) {
			if(i->second != NULL)
				delete i->second;
		}
		delete verticalDB;
		verticalDB = NULL;
	}
}

void transactionDatabase::dismantleItems() {
	if(items != NULL) {
		delete items;
		items = NULL;
	}
}