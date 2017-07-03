#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
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

	transactionDatabase * database = new transactionDatabase();
	database->loadFile(filepath);

	if(argc != 2) {
		cout << "Too few or too many arguments. Aborting ...\n";
	}
	else {
		algoFramework * algo = new algoFramework(atoi(argv[1]));		
		algo->runAlgo("out.put", database, 0.8);
	}


}