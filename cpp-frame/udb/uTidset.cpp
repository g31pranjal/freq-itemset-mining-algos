#include "uTidset.h"
#include <iostream>
#include <map>

using namespace std;

uTidset::uTidset(){
	this->support = 0;
}

uTidset::~uTidset(){

}

void uTidset::insert(int tid, double probability){
	if(tidAndProbMap.find(tid) == tidAndProbMap.end()){
		//cout << "meiyou" << endl;
		tidAndProbMap.insert(pair<int, double>(tid, probability));
		support += probability;
	}else{
		cout << "already exists" << endl;
	}
}

void uTidset::print(){
	for (map<int, double>::iterator it = tidAndProbMap.begin(); it != tidAndProbMap.end(); it++){
		cout << "Tid: " << it->first << " (" << it->second <<") ";
	}
	cout << "Support: " << support << endl;
}