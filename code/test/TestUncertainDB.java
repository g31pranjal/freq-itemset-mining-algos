package code.test;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URL;

import code.input.transaction_database_list_integers.UncertainTransactionDatabase;
import code.input.transaction_database_list_integers.UncertainItemset;
import code.input.transaction_database_list_integers.UItem;
import code.input.transaction_database_list_integers.TidAndProb;
import code.algorithms.frequentpatterns.UncertainAlgoFramework;


public class TestUncertainDB {

    public static void main(String args[]) throws IOException{

        String filename = "../../datasets/uncertainSample.txt";
        URL url = TestUncertainDB.class.getResource(filename);
        String input = java.net.URLDecoder.decode(url.getPath(),"UTF-8");

        // fetching the database.
        UncertainTransactionDatabase database = new UncertainTransactionDatabase();
        try {
            database.loadFile(input);
        } catch (IOException e) {
            e.printStackTrace();
        }
        database.printDatabase();

        UncertainAlgoFramework algo = new UncertainAlgoFramework();
        algo.runAlgo(null, database,0.05);
        // printing the horizontal database.
        // database.printHorizontalDatabase();

        // printing the horizontal database.
        // database.printVerticalDatabase();


        // running the framework
        //algoFramework algo = new algoFramework(Integer.parseInt(args[0]));
        //algo.runAlgo(null, database, 0.005);

    }

}