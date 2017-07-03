#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "transactionDatabase.h"

using namespace std;


string getFilepath(char filename[]) {

	char path[1000];
	memset(path, '\0', 1000);

	getcwd(path, 1000);
	realpath(filename, path); 

	return string(path);

}


int main() {

	string filepath = getFilepath("../datasets/mushrooms.txt");

	transactionDatabase * database = new transactionDatabase();
	database->loadFile(filepath);

	// algoFramework algo = new algoFramework(Integer.parseInt(args[0]));
	// 	algo.runAlgo("out.put", database, 0.1);

	
}