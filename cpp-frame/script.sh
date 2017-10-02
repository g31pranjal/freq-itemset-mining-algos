a="0.00"
b="massif.out."
for number in 9 8 7 6 5 4 3 2 1
do
	c=$a$number
	outputfile=$b$c
    valgrind --tool=massif --max-snapshots=500 --massif-out-file=./massif/switch_t20i6d100k/$outputfile ./switch 0 $c > ./massif/switch_t20i6d100k/$c.txt
    valgrind --tool=massif --max-snapshots=500 --massif-out-file=./massif/eclat_t20i6d100k/$outputfile ./switch 1 $c > ./massif/eclat_t20i6d100k/$c.txt
    valgrind --tool=massif --max-snapshots=500 --massif-out-file=./massif/viper_t20i6d100k/$outputfile ./switch 2 $c > ./massif/viper_t20i6d100k/$c.txt
done
