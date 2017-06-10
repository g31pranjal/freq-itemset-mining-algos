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

public class TransactionDatabase {

	private final Set<Integer> items = new HashSet<Integer>();
	private final List<List<Integer>> transactions = new ArrayList<List<Integer>>();
	
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
	}

	private void addTransaction(String itemsString[]) {
		
		List<Integer> itemset = new ArrayList<Integer>();
		
		for (String attribute : itemsString) {
			int item = Integer.parseInt(attribute);
			itemset.add(item); 
			items.add(item);
		}
		transactions.add(itemset);
	}

	public void printDatabase() {
		System.out.println("... Transaction Database :: (horizontal)");
		int count = 0; 
		
		for (List<Integer> itemset : transactions) { 
			System.out.print(count + " : ");

			StringBuilder r = new StringBuilder();
			for (Integer item : itemset) {
				r.append(item.toString());
				r.append(' ');
			}
			System.out.println(r);
			
			count++;
		}
	}

	// number of transactions (n) 
	public int size() {
		return transactions.size();
	}

	// list of transactions
	public List<List<Integer>> getTransactions() {
		return transactions;
	}
	
	// list of all the items 
	public Set<Integer> getItems() {
		return items;
	}
	
	// number of items (m)
	public int getItemSize() {
		return items.size();
	}
}
