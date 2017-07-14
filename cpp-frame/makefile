
all:
	g++ -g -std=c++11 -c transactionDatabase.cpp algoFramework.cpp -w
	g++ -g -std=c++11 alterDB.cpp transactionDatabase.o algoFramework.o -w -o switch

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf switch
	rm -rf out*

run:
	# ./switch 0 0.9
	# ./switch 0 0.85
	# ./switch 0 0.8
	# ./switch 0 0.75
	# ./switch 0 0.7
	# ./switch 0 0.65
	# ./switch 0 0.6
	# ./switch 0 0.55
	# ./switch 0 0.5
	# ./switch 0 0.45
	# ./switch 0 0.4
	# ./switch 0 0.35
	# ./switch 0 0.3
	# ./switch 0 0.25
	valgrind --tool=massif --massif-out-file=massif.out.02 ./switch 0 0.2
