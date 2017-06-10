package code.algorithms.frequentpatterns;

import java.util.HashMap;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.HashSet;
import java.util.Set;

import code.input.transaction_database_list_integers.TransactionDatabase;
import code.patterns.itemset_array_integers_with_count.Itemset;
import code.patterns.itemset_array_integers_with_count.Itemsets;

public class algoFramework {

	protected int minSupRelative;
	protected TransactionDatabase database;
	BufferedWriter writer = null; 
	protected Itemsets frequentItemsets;
	protected int itemsetCount;

	public void runAlgo(String outputFile, TransactionDatabase database, double minsupp) throws IOException {

		this.database = database;
		this.minSupRelative = (int) Math.ceil(minsupp * database.size());

		if(outputFile == null) {
			writer = null;
			frequentItemsets =  new Itemsets("FREQUENT ITEMSETS");
	    } 
	    else {
	    	frequentItemsets = null;
			writer = new BufferedWriter(new FileWriter(outputFile)); 
		}

		itemsetCount = 0;

		


		/** CALCULATES THE TIDSETS AND FILTERS INFREQUENT ONES. it can be passed to the recursive function for ECLAT **/

		// convert to vertical datastructure.
		int maxItemId = -1;
		final Map<Integer, Set<Integer>> mapItemCount = new HashMap<Integer, Set<Integer>>();
		for (int i = 0; i < database.size(); i++) {
			for (Integer item : database.getTransactions().get(i)) {
				Set<Integer> set = mapItemCount.get(item);
				if (set == null) {
					set = new HashSet<Integer>();
					mapItemCount.put(item, set);
					if (item > maxItemId) {
						maxItemId = item;
					}
				}
				set.add(i); 
			}
		}

		List<Integer> frequentItems = new ArrayList<Integer>();
		List<Set<Integer> > frequentTidsets = new ArrayList<Set<Integer>>();
	
		// get the list of frequent 1-itemsets.		
		for(Entry<Integer, Set<Integer>> entry : mapItemCount.entrySet()) {
			Set<Integer> tidset = entry.getValue();
			int support = tidset.size();
			int item = entry.getKey();
			if(support >= minSupRelative) {
				frequentItems.add(item);
				// saveSingleItem(item, tidset, tidset.size());
			}
		}

		// sort the list of frequen 1-itemsets.
		Collections.sort(frequentItems, new Comparator<Integer>() {
			@Override
			public int compare(Integer arg0, Integer arg1) {
				return mapItemCount.get(arg0).size() - mapItemCount.get(arg1).size();
			}
		});

		// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
		for(Integer item : frequentItems) {
			frequentTidsets.add(mapItemCount.get(item)); 
		} 

		// System.out.println("\nfrequent 1-itemset.\n");
		// for(int i=0;i<frequentItems.size();i++) {
		// 	System.out.println(frequentItems.get(i)+ " : "+frequentTidsets.get(i));
		// }

		






	}
	

	
}