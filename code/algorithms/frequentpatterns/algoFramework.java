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
import java.util.BitSet;

import code.input.transaction_database_list_integers.TransactionDatabase;
import code.patterns.itemset_array_integers_with_count.Itemset;
import code.patterns.itemset_array_integers_with_count.Itemsets;

public class algoFramework {

	protected int minSupRelative;
	protected TransactionDatabase database;
	protected Map<Integer, Set<Integer>> verticalDB;
	BufferedWriter writer = null; 
	protected Itemsets frequentItemsets;
	protected int itemsetCount;

	public void runAlgo(String outputFile, TransactionDatabase database, double minsupp) throws IOException {

		this.database = database;
		this.minSupRelative = (int) Math.ceil(minsupp * database.getN());
		this.verticalDB = database.getVerticalDB();

		if(outputFile == null) {
			writer = null;
			frequentItemsets =  new Itemsets("FREQUENT ITEMSETS");
	    } 
	    else {
	    	frequentItemsets = null;
			writer = new BufferedWriter(new FileWriter(outputFile)); 
		}

		itemsetCount = 0;
		
		List<Integer> oneFrequentItems = new ArrayList<Integer>();
		Long totalInOneFrequent = 0l;

		// get the list of frequent 1-itemsets.		
		for(Entry<Integer, Set<Integer>> entry : verticalDB.entrySet()) {
			Set<Integer> tidset = entry.getValue();
			int support = tidset.size();
			int item = entry.getKey();
			if(support >= minSupRelative) {
				oneFrequentItems.add(item);
				totalInOneFrequent += tidset.size();
			}
		}

		Integer avgTidsetSize = (int)(totalInOneFrequent / oneFrequentItems.size());

		// sort the list of frequent 1-itemsets.
		Collections.sort(oneFrequentItems, new Comparator<Integer>() {
			@Override
			public int compare(Integer arg0, Integer arg1) {
				return verticalDB.get(arg0).size() - verticalDB.get(arg1).size();
			}
		});

		/* 
		 *	logic to decide the algorithm to begin with 
			we can work with some kind of problistic model to determine, with some success, the startting algorithm
		*/

		Integer ECLATthreshold  = (int)(database.getN()*(1.0/4.0));
		Integer DECLATthreshold = (int)(database.getN()*(3.0/4.0));

		System.out.println("\nECLAT threshold : " + ECLATthreshold + ", DECLAT threshold : " + DECLATthreshold);
		System.out.println("Average value : " + avgTidsetSize);

		if(avgTidsetSize <= ECLATthreshold )
			this.constructTIDSETS(oneFrequentItems);
		else if(avgTidsetSize <= DECLATthreshold)
			this.constructBITSETS(oneFrequentItems);
		else
			this.constructDIFFSETS(oneFrequentItems);
 
	}



	public void constructTIDSETS(List<Integer> equivalenceClassItems) {
		
		List<Set<Integer> > equivalenceClassTidsets = new ArrayList<Set<Integer>>();
	
		// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
		for(Integer item : equivalenceClassItems) {
			equivalenceClassTidsets.add(verticalDB.get(item)); 
		} 

		System.out.println("\nfrequent 1-itemsets (using TIDSET)\n");
		for(int i=0;i<equivalenceClassItems.size();i++) {
			System.out.println(equivalenceClassItems.get(i)+ " : "+equivalenceClassTidsets.get(i));
		}

	}


	public void constructDIFFSETS(List<Integer> equivalenceClassItems) {
		
		List<Set<Integer> > equivalenceClassDiffsets = new ArrayList<Set<Integer>>();

		// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
		for(Integer item : equivalenceClassItems) {
			
			Set<Integer> tidset = verticalDB.get(item);
			Set<Integer> diffset = new HashSet<Integer>();
			Set<Integer> items = database.getItems();

			for(Integer i : items) {
				if(!tidset.contains(i)) {
					diffset.add(i);
				}
			}
			equivalenceClassDiffsets.add(diffset); 
		} 

		System.out.println("\nfrequent 1-itemsets (using DIFFSET).\n");
		for(int i=0;i<equivalenceClassItems.size();i++) {
			System.out.println(equivalenceClassItems.get(i)+ " : "+equivalenceClassDiffsets.get(i));
		}

	}

	public void constructBITSETS(List<Integer> equivalenceClassItems) {
		
		List<BitSetSupport> equivalenceClassBitsets = new ArrayList<BitSetSupport>();
	
		// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
		for(Integer item : equivalenceClassItems) {
			
			BitSetSupport bs = new BitSetSupport();
			for(Integer i : equivalenceClassItems) {
				bs.bitset.set(i);
				bs.support++;
			}
			equivalenceClassBitsets.add(bs);
		}

		System.out.println("\nfrequent 1-itemset (using BITSET)\n");
		for(int i=0;i<equivalenceClassItems.size();i++) {
			System.out.println(equivalenceClassItems.get(i)+ " : "+equivalenceClassBitsets.get(i));
		}

	}







	public class BitSetSupport{
		BitSet bitset = new BitSet();
		int support;
	}


	
	

	
}