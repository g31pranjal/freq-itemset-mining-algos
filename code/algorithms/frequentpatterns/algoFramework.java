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

	private void processEquivalenceClassEclat(int[] prefix, int prefixLength, int supportPrefix,
										 List<Integer> equivalentClassItems,
										 List<Set<Integer>> equivalentClassTidsets) throws IOException{
		int length = prefixLength + 1;

		// if there is only one itemset in equivalence class
		if (equivalentClassItems.size() == 1){
			int itemI = equivalentClassItems.get(0);
			Set<Integer> tidsetItemset = equivalentClassTidsets.get(0);

			// Then, we just save that itemset to file and stop.
			// To save the itemset we call the method save with the prefix "prefix" and the suffix "itemI"
			int support = calculateSupportEclat(length, supportPrefix, tidsetItemset);
			save(prefix, prefixLength, itemI, tidsetItemset, support);
			return;
		}

		// if there is only 2 itemsets in the equivalence class
		if (equivalentClassItems.size() == 2){
			// We get the suffix of the first itemset (an item that we will call I)
			int itemI = equivalentClassItems.get(0);
			Set<Integer> tidsetI = equivalentClassTidsets.get(0);
			int supportI = calculateSupportEclat(length, supportPrefix, tidsetI);
			save(prefix, prefixLength, itemI, tidsetI, supportI);

			//We get hte suffix of the second itemset (an item that we will call J)
			int itemJ = equivalentClassItems.get(1);
			Set<Integer> tidsetJ = equivalentClassTidsets.get(1);
			int supportJ = calculateSupportEclat(length, supportPrefix, tidsetJ);
			save(prefix, prefixLength, itemJ, tidsetJ, supportJ);

			// We calculate the tidset of the itemset resulting from the union of the first itemset and the second itemset
			Set<Integer> tidsetIJ = this.performAND(tidsetI, tidsetI.size(), tidsetJ, tidsetJ.size());

			int supportIJ = calculateSupportEclat(length, supportI, tidsetIJ);
			// We save the itemset prefix+IJ to the output
			if (supportIJ >= minSupRelative) {
				// Append the prefix with I
				int newPrefixLength = prefixLength + 1;
				prefix[prefixLength] = itemI;

				// We save the itemset prefix+IJ to the output
				save(prefix, newPrefixLength, itemJ, tidsetIJ, supportIJ);
			}
			return;
		}

		// The next loop combines each pairs of itemsets of the equivalence class
		// to form larger itemsets

		// For each itemset "prefix" + "i"
		for (int i = 0; j < equivalentClassItems.size(); i++){
			int suffixI = equivalentClassItems.get(i);
			// get the tidset and support of that itemset prefix + "i"
			Set<Integer> tidsetI = equivalentClassTidsets.get(i);
			int supportI = calculateSupportEclat(length, supportPrefix, tidsetI);
			// save the itemset to the file because it is frequent
			save(prefix, prefixLength, suffixI, tidsetI, supportI);

			// create the empty equivalence class for storing all itemsets of the equivalence class starting with prefix + i
			List<Integer> equivalenceClassISuffixItems

			// We will now calculate the tidset of the itemset {prefix, i, j}
			// This is done by intersecting the tid

		}
	}

	// calculate the support of an itemset X using the tidset of X.
	int calculateSupportEclat(int lengthOfX, int supportPrefix, Set<Integer> tidsetI){
		return tidsetI.size();
	}

	// Save an itemset to disk or memory (depending on what the user chose)
	private void save(int[] prefix, int prefixLength, int suffixItem, Set<Integer> tidset, int support) throws IOException {
		// increase the itemset count
		itemsetCount++;
		// if the result should be saved to memory
		if (writer == null){
			// append the prefix with the suffix
			int[] itemsetArray = new int[prefixLength+1];
			System.arraycopy(prefix, 0, itemsetArray, 0, prefixLength);
			itemsetArray[prefixLength] = suffixItem;
			// create an object "Itemset" and add it to the set of frequent itemsets
			Itemset itemset = new Itemset(itemsetArray);
			itemset.setAbsoluteSupport(support);
			frequentItemsets.addItemset(itemset, itemset.size());
		}else{
			// if the result should be saved to a file
			// write it to the output file
			StringBuilder buffer = new StringBuilder();
			for (int i = 0; i < prefixLength; i++){
				int item = prefix[i];
				buffer.append(item);
				buffer.append(" ");
			}
			buffer.append(suffixItem);
			// as well as its support
			buffer.append(" #SUP: ");
			buffer.append(support);
			if (showTransactionIdentifiers){
				buffer.append(" #TID:");
				for (Integer tid : tidset){
					buffer.append(" " + tid);
				}
			}
			writer.write(buffer.toString());
			writer.newLine();
		}
	}


}
