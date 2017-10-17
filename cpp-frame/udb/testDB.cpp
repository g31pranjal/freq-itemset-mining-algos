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


    string filepath = getFilepath("../../datasets/spmf(uncertain)/testUncertainData5uci/mushroom_10_100SPMF.txt");

	chrono::system_clock::time_point tp1 = chrono::system_clock::now();


	uTransactionDatabase * database = new uTransactionDatabase();
	database->loadFile(filepath);

	//database->printVerticalDatabase();
	
	chrono::system_clock::time_point tp2 = chrono::system_clock::now();
	
	if(argc != 4 && argc != 3 ) {
		cout << "Too few or too many arguments. Aborting ...\n";
	}
	else {

        int precision;
        if (argc == 3)
            precision = 16;
        else
            precision = atoi(argv[3]);

        if(precision > 24 || precision < 6){
            cout << "precision should be larger than 6 and smaller than 24" <<endl;
            return 0;
        }

		double minsup = atof(argv[2]);
		cout << "the minimum support is: "<< minsup << endl;
        cout << "the uBitset precision is " << precision <<endl;

        ofstream st1;
        st1.open("out.put", _Ios_Openmode::_S_app);
        st1 << "algo: " << argv[1] <<", support: " << argv[2] << ", precision: " << precision << "\n";
        st1.close();

	 	uAlgoFramework * algo = new uAlgoFramework(atoi(argv[1]));		
		algo->runAlgo("out.put", database, minsup, precision);

		delete algo;
	}

	chrono::system_clock::time_point tp3 = chrono::system_clock::now();

	chrono::duration<double> diff1 = tp2 - tp1;
	chrono::duration<double> diff2 = tp3 - tp2;

    string time_string = "";

	time_string += " Time taken to the database: " + to_string(diff1.count()) + " s\n";
	time_string += " Time taken to run the algorithm: " + to_string(diff2.count()) + " s\n\n" + "\n";

    ofstream st2;
    st2.open("out.put", _Ios_Openmode::_S_app);
    st2 << time_string;
    st2.close();

    cout << time_string;

}
