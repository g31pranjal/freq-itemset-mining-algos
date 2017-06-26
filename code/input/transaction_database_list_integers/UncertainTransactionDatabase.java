package code.input.transaction_database_list_integers;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.Map;
import java.util.HashMap;
import java.util.Map.Entry;


public class UncertainTransactionDatabase{
    // this is the set of items in the database
    private final Set<Integer> allItems = new HashSet<Integer>();
    // this is the list of transactions in the database
    private final List<UncertainItemset> transactions = new ArrayList<UncertainItemset>();
    private final Map<Integer, Set<TidAndProb>> verticalDB = new HashMap<Integer, Set<TidAndProb>>();

    /**
     * Load a transaction database from a file.
     * @param path the path of the file
     * @throws IOException exception if error while reading the file.
     */
    public void loadFile(String path) throws IOException {
        String thisLine;
        BufferedReader myInput = null;
        try {
            FileInputStream fin = new FileInputStream(new File(path));
            myInput = new BufferedReader(new InputStreamReader(fin));
            // for each transaction (line) in the input file
            while ((thisLine = myInput.readLine()) != null) {
                // if the line is  a comment, is  empty or is a
                // kind of metadata
                if (thisLine.isEmpty() == true ||
                        thisLine.charAt(0) == '#' || thisLine.charAt(0) == '%'
                        || thisLine.charAt(0) == '@') {
                    continue;
                }

                // process the transactionpackage code.algorithms.frequentpatterns;
                processTransactions(thisLine.split(" "));
            }
        } catch (Exception e) {
            // catch exceptions
            e.printStackTrace();
        } finally {
            if (myInput != null) {
                // close the file
                myInput.close();
            }
        }


        // convert it to vertical DB
        for (int i = 0; i < transactions.size(); i++) {
            //get the current transaction
            UncertainItemset currTransaction = transactions.get(i);
            // for each item in current transactionpackage code.algorithms.frequentpatterns;
            System.out.print("Current Transaction: "+(i)+" number of items is: "+ currTransaction.size()+" Items are: ");
            for (int j = 0; j < currTransaction.size(); j++ ) {
                Integer item = currTransaction.get(j).getId();
                System.out.print(item+" /");
                Set<TidAndProb> set = verticalDB.get(item);
                if (set == null) {
                    set = new HashSet<TidAndProb>();
                    verticalDB.put(item, set);
                }
                TidAndProb newPair = new TidAndProb(i+1, currTransaction.get(j).getProbability());
                set.add(newPair);
            }
            System.out.println();
        }
//        System.out.println("testing..........................");
//        System.out.println(verticalDB.size());
//        for (int i = 0; i < verticalDB.size(); i++) {
//            System.out.print("Item#: "+i+" ");
//            System.out.println(verticalDB.get(i));
//        }
//        for (Map.Entry<Integer, Set<TidAndProb>> entry:verticalDB.entrySet()) {
//            Integer itemID = entry.getKey();
//            System.out.print("Item#: "+itemID+" ");
//            System.out.println(entry.getValue());
//        }
    }

    private void processTransactions(String itemsString[]) {
        // We assume that there is no empty line

        // create a new itemset oject representing the transaction
        UncertainItemset transaction = new UncertainItemset();
        // for each item
        for (String itemString : itemsString) {
            // get the position of left parenthesis and right parenthesis
            int indexOfLeftParanthesis = itemString.indexOf('(');
            int indexOfRightParanthesis = itemString.indexOf(')');
            // get the item ID
            int itemID = Integer.parseInt(itemString.substring(0,
                    indexOfLeftParanthesis));
            // get the existential probability
            double value = Double.parseDouble(itemString.substring(
                    indexOfLeftParanthesis + 1, indexOfRightParanthesis));

            // create an item
            UItem item = new UItem(itemID, value);
            // add it to the transaction
            transaction.addItem(item);
            // add it to the set of all items
            allItems.add(item.getId());
        }
        // add the itemset to the transaction to the in-memory database
        transactions.add(transaction);
    }

    /**verticalDB
     * Print this database to System.out.
     */
    public void printDatabase() {
        System.out
                .println("===================  UNCERTAIN DATABASE ===================");
        int count = 0;
        // for each transaction
        for (UncertainItemset itemset : transactions) {
            // print the transaction
            System.out.print("0" + count + ":  ");
            itemset.print();
            System.out.println("");
            count++;
        }
    }

    /**
     * Get the number of transactions.
     * @return a int
     */
    public int size() {
        return transactions.size();
    }

    /**
     * Get the list of transactions.
     * @return the list of Transactions.
     */
    public List<UncertainItemset> getTransactions() {
        return transactions;
    }

    /**
     * Get the set of items in this database.
     * @return a Set of Integers
     */
    public Set<Integer> getAllItems() {
        return allItems;
    }

    public void printVerticalDB(){
        System.out.print("The Vertical Database.......................... # of items: ");
        System.out.println(verticalDB.size());

        for (Map.Entry<Integer, Set<TidAndProb>> entry:verticalDB.entrySet()) {
            Integer itemID = entry.getKey();
            System.out.print("Item#: "+itemID+" ");
            System.out.println(entry.getValue());
        }
    }

}