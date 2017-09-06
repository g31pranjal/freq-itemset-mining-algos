#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <unistd.h>

#include "uTransactionDatabase.h"
#include "uAlgoFramework.h"
//#include "uTidset.h"

using namespace std;

string getFilepath(char filename[]) {

	char path[1000];
	memset(path, '\0', 1000);

	getcwd(path, 1000);
	realpath(filename, path); 

	return string(path);

}

int main(int argc, char const *argv[]) {

//	string filepath = getFilepath("../../datasets/uncertainSample.txtmake");
	string filepath = getFilepath("../../datasets/spmf(uncertain)/testUncertainData5uci/mushroom_10_100SPMF.txt");
//	string filepath = getFilepath("../../datasets/spmf(uncertain)/testUncertainData5uci/connect4_10_100SPMF.txt");
//	string filepath = getFilepath("../../datasets/spmf(uncertain)/testUncertainData5uci/retail_10_100SPMF.txt");
	chrono::system_clock::time_point tp1 = chrono::system_clock::now();
	
	uTransactionDatabase * database = new uTransactionDatabase();
	database->loadFile(filepath);

	//database->printVerticalDatabase();
	
	chrono::system_clock::time_point tp2 = chrono::system_clock::now();
	
	if(argc != 3) {
		cout << "Too few or too many arguments. Aborting ...\n";
	}
	else {
		double minsup = atof(argv[2]);
		cout << "the minimum support is: "<< minsup << endl;

        ofstream st;
        st.open("out.put", _Ios_Openmode::_S_app);
        st << "\n\nalgo: " << argv[1] <<"   support: " << argv[2] << "\n";
        st.close();

	 	uAlgoFramework * algo = new uAlgoFramework(atoi(argv[1]));		
		algo->runAlgo("out.put", database, minsup);

		delete algo;
	}

	chrono::system_clock::time_point tp3 = chrono::system_clock::now();

	chrono::duration<double> diff1 = tp2 - tp1;
	chrono::duration<double> diff2 = tp3 - tp2;

	cout << " Time taken to the database: " << diff1.count() << " s\n";
	cout << " Time taken to run the algorithm: " << diff2.count() << " s\n\n" << endl;

}
