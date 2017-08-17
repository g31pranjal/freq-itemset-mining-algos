
all:
	g++ -g -std=c++11 -c transactionDatabase.cpp algoFramework.cpp  llSet.cpp llSet_element.cpp -w
	# g++ -g -std=c++11 -c llSet.cpp llSet_element.cpp -w
	g++ -g -std=c++11 alterDB.cpp transactionDatabase.o algoFramework.o llSet.o llSet_element.o -w -o switch
	# g++ -g -std=c++11 test.cpp llSet.o llSet_element.o -w

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf switch
	rm -rf out*

run:
	bash script.sh