import os 

path = os.getcwd();
print(path);

lineCount = 0;
wordCount = 0;

for filename in os.listdir(path):
	if filename.endswith(".txt"):
		# print("Found a text file");
		outputName = os.path.splitext(filename)[0];
		outputName = outputName +  "SPMF.txt";
		print(outputName);
		with open(filename) as f:
			of = open(outputName,"w+");
			lineCount = 0;
			for line in f:
				lineCount = lineCount+1;
				wordCount = 0;
				newLine = "";
				if (lineCount != 1):
					for word in line.split():
						wordCount = wordCount+1;
						if (wordCount > 2):
							if (wordCount%2 == 1):
								newLine = newLine + word;
							else:
								newLine = newLine +" ("+word+") ";
					# print(newLine);
					of.write(newLine+"\n");
			of.close();
			f.close();