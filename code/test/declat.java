package code.test;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URL;

import code.algorithms.frequentpatterns.eclat.AlgoDEclat;
import code.input.transaction_database_list_integers.TransactionDatabase;


/**
 * Example of how to use dECLAT algorithm from the source code.
 * @author Philippe Fournier-Viger - 2009
 */
public class declat {

	public static void main(String [] arg) throws IOException{
		
		// the file paths
		String input = fileToPath("../../datasets/retail.txt");  // the database
		String output = ".//output.txt";  // the path for saving the frequent itemsets found
		
		// minimum support
		double minsup = 0.001; // means a minsup of 2 transaction (we used a relative support)

		// Loading the transaction database
		TransactionDatabase database = new TransactionDatabase();
		try {
			database.loadFile(input);
		} catch (IOException e) {
			e.printStackTrace();
		}
//		context.printContext();
		
		// Applying the dECLAT algorithm
		AlgoDEclat algo = new AlgoDEclat();
		algo.runAlgorithm(output, database, minsup);
		// if you change use "true" in the line above, dECLAT will use
		// a triangular matrix  for counting support of itemsets of size 2.
		// For some datasets it should make the algorithm faster.
		
		algo.printStats();

	}
	
	public static String fileToPath(String filename) throws UnsupportedEncodingException{
		URL url = declat.class.getResource(filename);
		 return java.net.URLDecoder.decode(url.getPath(),"UTF-8");
	}
}
