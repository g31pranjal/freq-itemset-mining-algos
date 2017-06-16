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
			System.out.println("ECLAT");
		else if(avgTidsetSize <= DECLATthreshold)
			System.out.println("VIPER");
		else
			System.out.println("DECLAT");
			

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


	public void constructDIFFSETS(List<Integer> equivalenceClassItems) throws IOException{
		
		List<Set<Integer> > equivalenceClassDiffsets = new ArrayList<Set<Integer>>();

		// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
		for(Integer item : equivalenceClassItems) {
			
			Set<Integer> tidset = verticalDB.get(item);
			Set<Integer> diffset = new HashSet<Integer>();
			
			for(int i=0;i<database.getN();i++) {
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

		this.processEquivalenceClassDEclat(new int[1000], 0, database.getN(), equivalenceClassItems, equivalenceClassDiffsets);

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


	private void processEquivalenceClassDEclat( int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<Set<Integer>> equivalenceClassDiffsets) throws IOException {

		int length = prefixLength+1;
		
		if(equivalenceClassItems.size() == 1) {
			
			int item = equivalenceClassItems.get(0);
			Set<Integer> diffset = equivalenceClassDiffsets.get(0);
			//save(prefix, prefixLength, itemI, tidsetItemset, support);
			return;
		}

		if(equivalenceClassItems.size() == 2) {
			
			int itemI = equivalenceClassItems.get(0);
			Set<Integer> diffsetI = equivalenceClassDiffsets.get(0);
			int supportI = prefixSupport - diffsetI.size();
			// save(prefix, prefixLength, itemI, tidsetI, supportI);
			
			int itemJ = equivalenceClassItems.get(1);
			Set<Integer> diffsetJ = equivalenceClassDiffsets.get(1);
			int supportJ = prefixSupport - diffsetJ.size();
			// save(prefix, prefixLength, itemJ, tidsetJ, supportJ);
			
			Set<Integer> diffsetIJ = this.performDIFFERENCE(diffsetI, diffsetI.size(), diffsetJ, diffsetJ.size());
			int supportIJ = supportI - diffsetIJ.size();
			
			if(supportIJ >= minSupRelative) {
				int newPrefixLength = prefixLength+1;
				prefix[prefixLength] = itemI;
				//save(prefix, newPrefixLength, itemJ, tidsetIJ, supportIJ);
			
			}
			return;
		}

		int ETotal = 0;
		int DTotal = 0;

		for(int i=0; i< equivalenceClassItems.size(); i++) {
			

			int suffixI = equivalenceClassItems.get(i);
			// System.out.println(" i : "+suffixI);
			Set<Integer> diffsetI = equivalenceClassDiffsets.get(i);
			// System.out.println(" prefixSupport : "+prefixSupport);
			int supportI = prefixSupport - diffsetI.size();
			// save(prefix, prefixLength, suffixI, tidsetI, supportI);
			
			List<Integer> equivalenceClassISuffixItems= new ArrayList<Integer>();
			List<Set<Integer>> equivalenceClassIDiffsets = new ArrayList<Set<Integer>>();

			for(int j=i+1; j < equivalenceClassItems.size(); j++) {
				
				int suffixJ = equivalenceClassItems.get(j);
				// System.out.println(" j : "+suffixJ);
				Set<Integer> diffsetJ = equivalenceClassDiffsets.get(j);
				// System.out.println(" j : "+diffsetJ);
				int supportJ = prefixSupport - diffsetJ.size();
				
				Set<Integer> diffsetIJ = performDIFFERENCE(diffsetI, supportI, diffsetJ, supportJ);
				
				// System.out.println("\n diffset ij : "+diffsetIJ);


				int supportIJ = supportI - diffsetIJ.size();
					
				// System.out.println("support ij : "+supportI);

				if(supportIJ >= minSupRelative) {
					ETotal += supportIJ;
					DTotal += (supportI - supportIJ);
					equivalenceClassISuffixItems.add(suffixJ);
					equivalenceClassIDiffsets.add(diffsetIJ);
				}
			}
			
			if(equivalenceClassISuffixItems.size() > 0) {

				prefix[prefixLength] = suffixI;
				int newPrefixLength = prefixLength+1;

				// print before recursive class
				System.out.println("\nfrequent "+(newPrefixLength+1)+"-itemset : \n");
				for(int p=0;p<equivalenceClassISuffixItems.size();p++) {
					for(int k=0;k<newPrefixLength;k++) {
						System.out.print(prefix[k]);
					}
					System.out.println(","+equivalenceClassISuffixItems.get(p)+" : "+equivalenceClassIDiffsets.get(p));
				}

				double DAvg = DTotal / (double)equivalenceClassISuffixItems.size();
				double EAvg = ETotal / (double)equivalenceClassISuffixItems.size();
				
				int ECLATthreshold = (int)(database.getN()*(1.0/4.0) );
				int ECLATstart = 0;
				int DECLATthreshold  = supportI - ECLATthreshold;
				int DECLATstart  = supportI;

				System.out.println("ECLAT thr. : "+ECLATthreshold+" , "+ "DECLAT thr. : "+DECLATthreshold);
				System.out.println("ECLAT str. : "+ECLATstart+" , "+ "DECLAT str. : "+DECLATstart);
				System.out.println("E(avg) : "+EAvg);
				System.out.println("D(avg) : "+DAvg);

				if(DECLATthreshold <= ECLATthreshold) {
					double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
					System.out.println("POINT OF DIFF : " + PointOfDiff);
					if(EAvg > PointOfDiff)
						System.out.println("DECLAT");
					else 
						System.out.println("ECLAT");
				}
				else {
					if(EAvg <= ECLATthreshold)
						System.out.println("ECLAT");
					else if(EAvg <= DECLATthreshold)
						System.out.println("VIPER");
					else
						System.out.println("DECLAT");
				}

				processEquivalenceClassDEclat(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
				
			}
			
			break;

		}
		
    }

	
	// diff(PXY) = diff(PY) - DIFF(PX)	
	Set<Integer> performDIFFERENCE(Set<Integer> tidsetI, int supportI, Set<Integer> tidsetJ, int supportJ) {
		
		Set<Integer> diffsetIJ = new HashSet<Integer>();
		for(Integer tid : tidsetJ) 
			if(tidsetI.contains(tid) == false)
				diffsetIJ.add(tid);

		return diffsetIJ;
	}





	// private void processEquivalenceClassVIPER(int[] prefix, int prefixLength, int supportPrefix, List<Integer> equivalenceClassItems, List){


 //    }


	// private void processEquivalenceClassEclat(int[] prefix, int prefixLength, int supportPrefix,
	// 									 List<Integer> equivalenceClassItems,
	// 									 List<Set<Integer>> equivalenceClassTidsets) throws IOException{
	// 	int length = prefixLength + 1;

	// 	// if there is only one itemset in equivalence class
	// 	if (equivalenceClassItems.size() == 1){
	// 		int itemI = equivalenceClassItems.get(0);
	// 		Set<Integer> tidsetItemset = equivalenceClassTidsets.get(0);

	// 		// Then, we just save that itemset to file and stop.
	// 		// To save the itemset we call the method save with the prefix "prefix" and the suffix "itemI"
	// 		int support = calculateSupportEclat(length, supportPrefix, tidsetItemset);
	// 		save(prefix, prefixLength, itemI, tidsetItemset, support);
	// 		return;
	// 	}

	// 	// if there is only 2 itemsets in the equivalence class
	// 	if (equivalenceClassItems.size() == 2){
	// 		// We get the suffix of the first itemset (an item that we will call I)
	// 		int itemI = equivalenceClassItems.get(0);
	// 		Set<Integer> tidsetI = equivalenceClassTidsets.get(0);
	// 		int supportI = calculateSupportEclat(length, supportPrefix, tidsetI);
	// 		save(prefix, prefixLength, itemI, tidsetI, supportI);

	// 		//We get hte suffix of the second itemset (an item that we will call J)
	// 		int itemJ = equivalenceClassItems.get(1);
	// 		Set<Integer> tidsetJ = equivalenceClassTidsets.get(1);
	// 		int supportJ = calculateSupportEclat(length, supportPrefix, tidsetJ);
	// 		save(prefix, prefixLength, itemJ, tidsetJ, supportJ);

	// 		// We calculate the tidset of the itemset resulting from the union of the first itemset and the second itemset
	// 		Set<Integer> tidsetIJ = this.performAND(tidsetI, tidsetI.size(), tidsetJ, tidsetJ.size());

	// 		int supportIJ = calculateSupportEclat(length, supportI, tidsetIJ);
	// 		// We save the itemset prefix+IJ to the output
	// 		if (supportIJ >= minSupRelative) {
	// 			// Append the prefix with I
	// 			int newPrefixLength = prefixLength + 1;
	// 			prefix[prefixLength] = itemI;

	// 			// We save the itemset prefix+IJ to the output
	// 			save(prefix, newPrefixLength, itemJ, tidsetIJ, supportIJ);
	// 		}
	// 		return;
	// 	}

	// 	// The next loop combines each pairs of itemsets of the equivalence class
	// 	// to form larger itemsets

	// 	// For each itemset "prefix" + "i"
	// 	for (int i = 0; j < equivalenceClassItems.size(); i++){
	// 		int suffixI = equivalenceClassItems.get(i);
	// 		// get the tidset and support of that itemset prefix + "i"
	// 		Set<Integer> tidsetI = equivalenceClassTidsets.get(i);
	// 		int supportI = calculateSupportEclat(length, supportPrefix, tidsetI);
	// 		// save the itemset to the file because it is frequent
	// 		save(prefix, prefixLength, suffixI, tidsetI, supportI);

	// 		// create the empty equivalence class for storing all itemsets of the equivalence class starting with prefix + i
	// 		List<Integer> equivalenceClassISuffixItems = new ArrayList<Integer>();
	// 		List<Set<Integer>> equivalenceITidsets = new ArrayList<Set<Integer>>();

	// 		// For each itemset "prefix" + j
	// 		for (int j=i+1; j < equivalenceClassItems.size(); j++){
	// 			int suffixJ = equivalenceClassItems.get(j);

	// 			// Get the tidset and support of the itemset prefix + "j"
	// 			Set<Integer> tidsetJ = equivalenceClassTidsets.get(j);
	// 			int supportJ = calculateSupportEclat(length, supportPrefix, tidsetJ);

	// 			// We will now calculate the tidset of the itemset {prefix, i,j}
	// 			// This is done by intersecting the tidset of the itemset prefix + i
	// 			// with the itemset prefix+j
	// 			Set<Integer> tidsetIJ = performANDEclat(tidsetI, supportI, tidsetJ, supportJ);
	// 			int supportIJ = calculateSupportEclat(length, supportI, tidsetIJ);
	// 			// If the itemset prefix+i+j is frequent, then we add it to the equivalence class of itemsets having the prefix
	// 			// "prefix" + i.
	// 			// Note actually, we just keep "j" for optimization because all itemsets in the equivalence class of prefix + i
	// 			// will start with prefix + i so it would just waste memory to keep prefix + i for all itemsets
	// 			if (supportIJ >= minSupRelative) {
	// 				equivalenceClassISuffixItems.add(suffixJ);
	// 				// We also keep the corresponding tidset
	// 				equivalenceITidsets.add(tidsetIJ);
	// 			}
	// 		}

	// 		// If there is more than an itemset in the equivalence class
	// 		// then we recursively process that equivalence class to find larger itemsets
	// 		if (equivalenceClassISuffixItems.size() > 0){
	// 			// We create the itemset prefix + i
	// 			prefix[prefixLength] = suffixI;
	// 			int newPrefixLength = prefixLength+1;

	// 			// Recursive call
	// 			processEquivalenceClassEclat(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassTidsets);
	// 		}

	// 	}



	public class BitSetSupport{
		BitSet bitset = new BitSet();
		int support;
	}


	
	// // calculate the support of an itemset X using the tidset of X.
	// int calculateSupportEclat(int lengthOfX, int supportPrefix, Set<Integer> tidsetI){
	// 	return tidsetI.size();
	// }

	// // Save an itemset to disk or memory (depending on what the user chose)
	// private void save(int[] prefix, int prefixLength, int suffixItem, Set<Integer> tidset, int support) throws IOException {
	// 	// increase the itemset count
	// 	itemsetCount++;
	// 	// if the result should be saved to memory
	// 	if (writer == null){
	// 		// append the prefix with the suffix
	// 		int[] itemsetArray = new int[prefixLength+1];
	// 		System.arraycopy(prefix,performAND 0, itemsetArray, 0, prefixLength);
	// 		itemsetArray[prefixLength] = suffixItem;
	// 		// create an object "Itemset" and add it to the set of frequent itemsets
	// 		Itemset itemset = new Itemset(itemsetArray);
	// 		itemset.setAbsoluteSupport(support);
	// 		frequentItemsets.addItemset(itemset, itemset.size());
	// 	}else{
	// 		// if the result should be saved to a file
	// 		// write it to the output file
	// 		StringBuilder buffer = new StringBuilder();
	// 		for (int i = 0; i < prefixLength; i++){
	// 			int item = prefix[i];
	// 			buffer.append(item);
	// 			buffer.append(" ");
	// 		}
	// 		buffer.append(suffixItem);
	// 		// as well as its support
	// 		buffer.append(" #SUP: ");
	// 		buffer.append(support);
	// 		if (showTransactionIdentifiers){
	// 			buffer.append(" #TID:");
	// 			for (Integer tid : tidset){
	// 				buffer.append(" " + tid);
	// 			}
	// 		}
	// 		writer.write(buffer.toString());
	// 		writer.newLine();
	// 	}
	// }

	// Set<Integer> performANDEclat(){

 //    }


}
