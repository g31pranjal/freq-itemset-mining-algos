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
    private final Set<Integer> items = new HashSet<Integer>();
    private final List<List<Integer>> horizontalDB = new ArrayList<List<Integer>>();
    private final Map<Integer, Set<Integer>> verticalDB = new HashMap<Integer, Set<Integer>>();



    public void loadFile(String path) throws IOException {

        String thisLine;
        BufferedReader myInput = null;

        // opening the line and reading each line
        try {
            FileInputStream fin = new FileInputStream(new File(path));
            myInput = new BufferedReader(new InputStreamReader(fin));
            while ((thisLine = myInput.readLine()) != null) {
                if (thisLine.isEmpty() == false &&
                        thisLine.charAt(0) != '#' && thisLine.charAt(0) != '%' && thisLine.charAt(0) != '@') {
                    addTransaction(thisLine.split(" "));
                }
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        finally {
            if (myInput != null) {
                myInput.close();
            }
        }

        // convert to vertical datastructure.

        for (int i = 0; i < this.getN(); i++) {
            for (Integer item : this.getHorizontalDB().get(i)) {
                Set<Integer> set = verticalDB.get(item);
                if (set == null) {
                    set = new HashSet<Integer>();
                    verticalDB.put(item, set);
                }
                set.add(i);
            }
        }

    }

    private void addTransaction(String itemsString[]) {

        List<Integer> itemset = new ArrayList<Integer>();

        for (String attribute : itemsString) {
            int item = Integer.parseInt(attribute);
            itemset.add(item);
            items.add(item);
        }
        horizontalDB.add(itemset);
    }


    // prints the horizontal database
    public void printHorizontalDatabase() {
        System.out.println("\n... Transaction Database :: (horizontal)\n");
        int count = 0;

        for (List<Integer> itemset : horizontalDB) {
            System.out.println((count++) + " : " + itemset);
        }
        System.out.println("# transactions (n) " + this.getN() + ", # items (m) : " + this.getM());
    }


    // prints the vertical database
    public void printVerticalDatabase() {
        System.out.println("\n... Transaction Database :: (vertical)\n");
        for(Entry<Integer, Set<Integer>> entry : verticalDB.entrySet()) {
            System.out.println(entry.getKey() + " : " + entry.getValue());
        }
        System.out.println("# transactions (n) " + this.getN() + ", # items (m) : " + this.getM());
    }


    // number of horizontalDB (n)
    public int getN() {
        return horizontalDB.size();
    }

    public int size() {
        return horizontalDB.size();
    }

    // list of horizontalDB
    public List<List<Integer>> getHorizontalDB() {
        return horizontalDB;
    }


    // list of verticalDB
    public Map<Integer, Set<Integer>> getVerticalDB() {
        return verticalDB;
    }


    // list of all the items
    public Set<Integer> getItems() {
        return items;
    }


    // number of items (m)
    public int getM() {
        return items.size();
    }
}