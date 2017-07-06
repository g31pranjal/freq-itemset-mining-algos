#include "UTransactionDatabase.h"

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <unistd.h>

using namespace std;

string getFilepath(char filename[]) {

	char path[1000];
	memset(path, '\0', 1000);

	getcwd(path, 1000);
	realpath(filename, path); 

	return string(path);

}

int main(int argc, char const *argv[])
{
	string filepath = getFilepath("../../datasets/uncertainSample.txt");
	UTransactionDatabase * database = new UTransactionDatabase();
	database->loadFile(filepath);
	//database->printHorizontalDatabase();
	database->printVerticalDatabase();
	delete database;
	return 0;
}