
all:
	g++ -g -std=c++11 -c transactionDatabase.cpp algoFramework.cpp -w
	g++ -g -std=c++11 alterDB.cpp transactionDatabase.o algoFramework.o -w -o switch

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf switch
	rm -rf out*

run:

	valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.002 ./switch 0 0.02 > ./massif/switch_retail/output002.txt
	valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.0019 ./switch 0 0.019 > ./massif/switch_retail/output0019.txt
	valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.0018 ./switch 0 0.018 > ./massif/switch_retail/output0018.txt
	valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.0017 ./switch 0 0.017 > ./massif/switch_retail/output0017.txt
	valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.0016 ./switch 0 0.016 > ./massif/switch_retail/output0016.txt
	valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.0015 ./switch 0 0.015 > ./massif/switch_retail/output0015.txt
	valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.0014 ./switch 0 0.014 > ./massif/switch_retail/output0014.txt
	valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.0013 ./switch 0 0.013 > ./massif/switch_retail/output0013.txt

	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.012 ./switch 0 0.012 > ./massif/switch_retail/output0012.txt
	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.011 ./switch 0 0.011 > ./massif/switch_retail/output0011.txt
	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.010 ./switch 0 0.01 > ./massif/switch_retail/output0010.txt
	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.009 ./switch 0 0.009 > ./massif/switch_retail/output0009.txt
	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.008 ./switch 0 0.008 > ./massif/switch_retail/output0008.txt
	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.007 ./switch 0 0.007 > ./massif/switch_retail/output0007.txt
	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.006 ./switch 0 0.006 > ./massif/switch_retail/output0006.txt
	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.005 ./switch 0 0.005 > ./massif/switch_retail/output0005.txt
	# valgrind --tool=massif --massif-out-file=./massif/switch_retail/massif.out.004 ./switch 0 0.004 > ./massif/switch_retail/output0004.txt
	#valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.003 ./switch 0 0.03 > ./massif/switch/output003.txt
	#valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.002 ./switch 0 0.02 > ./massif/switch/output002.txt
	#valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.001 ./switch 0 0.01 > ./massif/switch/output001.txt

