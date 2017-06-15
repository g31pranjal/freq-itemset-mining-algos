package code.test;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URL;

import code.input.transaction_database_list_integers.TransactionDatabase;
import code.algorithms.frequentpatterns.algoFramework;


public class alterDBstructure {

	public static void main(String args[]) throws IOException{
		
		String filename = "../../datasets/test1.txt";
		URL url = alterDBstructure.class.getResource(filename);
		String input = java.net.URLDecoder.decode(url.getPath(),"UTF-8");

		// fetching the database.
		TransactionDatabase database = new TransactionDatabase();
		try {
			database.loadFile(input);
		} catch (IOException e) {
			e.printStackTrace();
		}

		// printing the horizontal database.
		database.printHorizontalDatabase();

		// printing the horizontal database.
		database.printVerticalDatabase();


		// running the framework
		algoFramework algo = new algoFramework();
		algo.runAlgo(null, database, 0.3);

	}

}


