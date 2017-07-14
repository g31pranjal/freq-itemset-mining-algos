
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
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.02 ./switch 2 0.2 > ./massif/viper/output02.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.019 ./switch 2 0.19 > ./massif/viper/output019.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.018 ./switch 2 0.18 > ./massif/viper/output018.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.017 ./switch 2 0.17 > ./massif/viper/output017.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.016 ./switch 2 0.16 > ./massif/viper/output016.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.015 ./switch 2 0.15 > ./massif/viper/output015.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.014 ./switch 2 0.14 > ./massif/viper/output014.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.013 ./switch 2 0.13 > ./massif/viper/output013.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.012 ./switch 2 0.12 > ./massif/viper/output012.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.011 ./switch 2 0.11 > ./massif/viper/output011.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.010 ./switch 2 0.1 > ./massif/viper/output010.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.009 ./switch 2 0.09 > ./massif/viper/output009.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.008 ./switch 2 0.08 > ./massif/viper/output008.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.007 ./switch 2 0.07 > ./massif/viper/output007.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.006 ./switch 2 0.06 > ./massif/viper/output006.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.005 ./switch 2 0.05 > ./massif/viper/output005.txt
	valgrind --tool=massif --massif-out-file=./massif/viper/massif.out.004 ./switch 2 0.04 > ./massif/viper/output004.txt
	#valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.003 ./switch 0 0.03 > ./massif/switch/output003.txt
	#valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.002 ./switch 0 0.02 > ./massif/switch/output002.txt
	#valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.001 ./switch 0 0.01 > ./massif/switch/output001.txt

