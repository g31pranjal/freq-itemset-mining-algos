
all:
	g++ -g -std=c++11 -c transactionDatabase.cpp algoFramework.cpp -w
	g++ -g -std=c++11 alterDB.cpp transactionDatabase.o algoFramework.o -w -o switch

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf switch
	rm -rf out*