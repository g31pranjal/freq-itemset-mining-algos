
all:
	g++ -g -std=c++11 -c transactionDatabase.cpp -w
	g++ -g -std=c++11 alterDB.cpp transactionDatabase.o -w -o alterDB

clean:
	rm -rf *.o
	rm -rf alterDB