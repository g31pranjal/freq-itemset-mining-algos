package code.test;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URL;

import code.algorithms.frequentpatterns.eclat.AlgoEclat_Bitset;
import code.input.transaction_database_list_integers.TransactionDatabase;


/**
 * Example of how to use ECLAT algorithm from the source code.
 * @author Philippe Fournier-Viger - 2009
 */
public class viper {

	public static void main(String [] arg) throws IOException{
		// Loading the binary context
		String input = fileToPath("../../datasets/retail.txt");  // the database
		String output = ".//output.txt";  // the path for saving the frequent itemsets found
		
		double minsup = 0.001; // means a minsup of 2 transaction (we used a relative support)
		
		TransactionDatabase database = new TransactionDatabase();
		try {
			database.loadFile(input);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		// Applying the ECLAT algorithm
		AlgoEclat_Bitset algo = new AlgoEclat_Bitset();
		algo.runAlgorithm(output, database, minsup);

		// NOTE 1: if you  use "true" in the line above, CHARM will use
		// a triangular matrix  for counting support of itemsets of size 2.
		// For some datasets it should make the algorithm faster.
		
		algo.printStats();

	}
	
	public static String fileToPath(String filename) throws UnsupportedEncodingException{
		URL url = viper.class.getResource(filename);
		 return java.net.URLDecoder.decode(url.getPath(),"UTF-8");
	}
}
