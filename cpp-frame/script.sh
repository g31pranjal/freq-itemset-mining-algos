a="0.000"
b="massif.out."
for number in 6 59 58 57 56 55 54 53 52 51 5
do
	c=$a$number
	outputfile=$b$number
    valgrind --tool=massif --max-snapshots=500 --massif-out-file=./massif/switch_BMS1/$outputfile ./switch 0 $c > ./massif/switch_BMS1/$number.txt
    # valgrind --tool=massif --max-snapshots=500 --massif-out-file=./massif/eclat_BMS1/$outputfile ./switch 1 $c > ./massif/switch_BMS1/$number.txt
    valgrind --tool=massif --max-snapshots=500 --massif-out-file=./massif/viper_BMS1/$outputfile ./switch 2 $c > ./massif/switch_BMS1/$number.txt
done
