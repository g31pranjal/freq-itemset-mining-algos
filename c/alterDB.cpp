#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <unistd.h>

#include "transactionDatabase.h"
#include "algoFramework.h"

using namespace std;


string getFilepath(char filename[]) {

	char path[1000];
	memset(path, '\0', 1000);

	getcwd(path, 1000);
	realpath(filename, path); 

	return string(path);

}

int main(int argc, char * argv[]) {

	string filepath = getFilepath("../datasets/mushrooms.txt");

	chrono::system_clock::time_point startTime = chrono::system_clock::now();

	transactionDatabase * database = new transactionDatabase();
	database->loadFile(filepath);

	if(argc != 2) {
		cout << "Too few or too many arguments. Aborting ...\n";
	}
	else {
		algoFramework * algo = new algoFramework(atoi(argv[1]));		
		algo->runAlgo("out.put", database, 0.7);
	
		delete algo;
	}


	chrono::system_clock::time_point endTime = chrono::system_clock::now();
	chrono::duration<double> diffTime = endTime - startTime;

	cout << " Time taken : " << diffTime.count() << " s\n\n" << endl;

}