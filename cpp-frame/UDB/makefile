all:
	g++ -g -std=c++11 -c UTransactionDatabase.cpp UItem.cpp TidAndProb.cpp -w
	g++ -g -std=c++11 testDB.cpp UTransactionDatabase.o UItem.o TidAndProb.o -w -o testDB

clean:
	rm -rf *.o
	rm -rf *.otu
	rm -rf testDB