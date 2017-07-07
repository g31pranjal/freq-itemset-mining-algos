#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
	
#include "algoFramework.h"
#include "transactionDatabase.h"

using namespace std;

static in INTSIZE = 32;

uAlgoFramework::uAlgoFramework(){
	this->algo = 0;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;
}

uAlgoFramework::uAlgoFramework(int algo){
	this->algo = algo;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;	
}