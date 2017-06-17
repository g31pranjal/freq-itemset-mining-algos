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

		if(oneFrequentItems.size() > 0) {

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

			if(avgTidsetSize <= ECLATthreshold ) {
				System.out.println("ECLAT");
				this.constructTIDSETS(oneFrequentItems);
			}
			else if(avgTidsetSize <= DECLATthreshold){
				System.out.println("VIPER");
				this.constructBITSETS(oneFrequentItems);
			}
			else{
				System.out.println("DECLAT");
				this.constructDIFFSETS(oneFrequentItems);
			}
		}
	}








	public void constructTIDSETS(List<Integer> equivalenceClassItems) throws IOException {
		
		List<Set<Integer> > equivalenceClassTidsets = new ArrayList<Set<Integer>>();
	
		// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
		for(Integer item : equivalenceClassItems) {
			equivalenceClassTidsets.add(verticalDB.get(item)); 
		} 

		// System.out.println("\nfrequent 1-itemsets (using TIDSET)\n");
		// for(int i=0;i<equivalenceClassItems.size();i++) {
		// 	System.out.println(equivalenceClassItems.get(i)+ " : "+equivalenceClassTidsets.get(i));
		// }

		Set<Integer> enot = new HashSet<Integer>();
		for(int i=0;i<database.getN();i++) 
			enot.add(i);

		this.processEquivalenceClassEclat(enot ,new int[1000], 0, database.getN(), equivalenceClassItems, equivalenceClassTidsets);
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

		// System.out.println("\nfrequent 1-itemsets (using DIFFSET).\n");
		// for(int i=0;i<equivalenceClassItems.size();i++) {
		// 	System.out.println(equivalenceClassItems.get(i)+ " : "+equivalenceClassDiffsets.get(i));
		// }

		this.processEquivalenceClassDEclat(new HashSet<Integer>(), new int[1000], 0, database.getN(), equivalenceClassItems, equivalenceClassDiffsets);
	}


	public void constructBITSETS(List<Integer> equivalenceClassItems) throws IOException {
		
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

		// System.out.println("\nfrequent 1-itemset (using BITSET)\n");
		// for(int i=0;i<equivalenceClassItems.size();i++) {
		// 	System.out.println(equivalenceClassItems.get(i)+ " : "+equivalenceClassBitsets.get(i));
		// }

		BitSetSupport enot = new BitSetSupport();
		for(int i=0;i<database.getN();i++) {
			enot.bitset.set(i);
			enot.support++;
		}

		this.processEquivalenceClassViper(enot, new int[1000], 0, database.getN(), equivalenceClassItems, equivalenceClassBitsets);
	}













	private void processEquivalenceClassDEclat(Set<Integer> parentDiffsUnion, int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<Set<Integer>> equivalenceClassDiffsets) throws IOException {

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
			
			Set<Integer> diffsetIJ = this.performDIFFERENCE(diffsetI, diffsetJ);
			int supportIJ = supportI - diffsetIJ.size();
			
			if(supportIJ >= minSupRelative) {
				int newPrefixLength = prefixLength+1;
				prefix[prefixLength] = itemI;
				//save(prefix, newPrefixLength, itemJ, tidsetIJ, supportIJ);
			
			}
			return;
		}


		for(int i=0; i< equivalenceClassItems.size(); i++) {
			
			int ETotal = 0;
			int DTotal = 0;

			int suffixI = equivalenceClassItems.get(i);
			Set<Integer> diffsetI = equivalenceClassDiffsets.get(i);
			int supportI = prefixSupport - diffsetI.size();
			// save(prefix, prefixLength, suffixI, tidsetI, supportI);
			
			List<Integer> equivalenceClassISuffixItems= new ArrayList<Integer>();
			List<Set<Integer>> equivalenceClassIDiffsets = new ArrayList<Set<Integer>>();

			for(int j=i+1; j < equivalenceClassItems.size(); j++) {
				
				int suffixJ = equivalenceClassItems.get(j);
				Set<Integer> diffsetJ = equivalenceClassDiffsets.get(j);
				int supportJ = prefixSupport - diffsetJ.size();
				
				Set<Integer> diffsetIJ = performDIFFERENCE(diffsetI, diffsetJ);
				int supportIJ = supportI - diffsetIJ.size();
					
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
				
				// printing in TIDSETS. Unwanted peice of code ----------------------------------------------------------
				
				// parentDiffsUnion.addAll(diffsetI);

				// List<Set<Integer>> equivalenceClassITidsets = convertDIFFSETStoTIDSETS(parentDiffsUnion, equivalenceClassIDiffsets);
				
				// System.out.println("\nfrequent "+(newPrefixLength+1)+"-itemset : \n");
				// for(int p=0;p<equivalenceClassISuffixItems.size();p++) {
				// 	for(int k=0;k<newPrefixLength;k++) {
				// 		System.out.print(prefix[k]);
				// 	}
				// 	System.out.println(","+equivalenceClassISuffixItems.get(p) +" : "+equivalenceClassITidsets.get(p));
				// }

				// ------------------------------------------------------------------------------------------------------

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
					if(EAvg > PointOfDiff){
						
						System.out.println("DECLAT");
						
						parentDiffsUnion.addAll(diffsetI);
						processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
					else {
						
						System.out.println("ECLAT");

						parentDiffsUnion.addAll(diffsetI);
						List<Set<Integer>> equivalenceClassITidsets = convertDIFFSETStoTIDSETS(parentDiffsUnion, equivalenceClassIDiffsets);
						Set<Integer> prefixTidset = formPrefixTidsetFromParentDiffsUnion(parentDiffsUnion);
						processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
				}
				else {
					if(EAvg <= ECLATthreshold){
						
						System.out.println("ECLAT");
						
						parentDiffsUnion.addAll(diffsetI);
						List<Set<Integer>> equivalenceClassITidsets = convertDIFFSETStoTIDSETS(parentDiffsUnion, equivalenceClassIDiffsets);
						Set<Integer> prefixTidset = formPrefixTidsetFromParentDiffsUnion(parentDiffsUnion);
						processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
					else if(EAvg <= DECLATthreshold){
						
						System.out.println("VIPER");
						
						parentDiffsUnion.addAll(diffsetI);
						List<BitSetSupport> equivalenceClassIBitsets = convertDIFFSETStoBITSETS(parentDiffsUnion, equivalenceClassIDiffsets);
						BitSetSupport prefixBitset = formPrefixBitsetFromParentDiffsUnion(parentDiffsUnion);
						this.processEquivalenceClassViper(prefixBitset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
					}
					else{
						
						System.out.println("DECLAT");
						
						parentDiffsUnion.addAll(diffsetI);
						processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
				}

				// parentDiffsUnion.addAll(diffsetI);
				// processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
				
			}
		}
	}

	// diff(PXY) = diff(PY) - DIFF(PX)	
	Set<Integer> performDIFFERENCE(Set<Integer> diffsetI, Set<Integer> diffsetJ) {
		
		Set<Integer> diffsetIJ = new HashSet<Integer>();
		for(Integer tid : diffsetJ) 
			if(diffsetI.contains(tid) == false)
				diffsetIJ.add(tid);

		return diffsetIJ;
	}

	List<Set<Integer>> convertDIFFSETStoTIDSETS(Set<Integer> parentDiffsUnion, List<Set<Integer>> equivalenceClassIDiffsets) {

		List<Set<Integer>> equivalenceClassITidsets = new ArrayList<Set<Integer>>();

		for(Set<Integer> diffset : equivalenceClassIDiffsets) {
			Set<Integer> tid = new HashSet<Integer>();
			for(int i=0;i<database.getN();i++) {
				if(!parentDiffsUnion.contains(i) && !diffset.contains(i))
					tid.add(i);
			}
			equivalenceClassITidsets.add(tid);
		}

		return equivalenceClassITidsets;
	}

	Set<Integer> formPrefixTidsetFromParentDiffsUnion(Set<Integer> parentDiffsUnion) {

		Set<Integer> prefixTidset = new HashSet<Integer>();
		for(int i=0;i<database.getN();i++) {
			if(!parentDiffsUnion.contains(i))
				prefixTidset.add(i);
		}

		return prefixTidset;
	}

	List<BitSetSupport> convertDIFFSETStoBITSETS(Set<Integer> parentDiffsUnion, List<Set<Integer>> equivalenceClassIDiffsets) {

		List<BitSetSupport> equivalenceClassIBitsets = new ArrayList<BitSetSupport>();

		for(Set<Integer> diffset : equivalenceClassIDiffsets) {
			BitSetSupport bs = new BitSetSupport();
			for(int i=0;i<database.getN();i++) {
				if(!parentDiffsUnion.contains(i) && !diffset.contains(i)) {
					bs.bitset.set(i);
					bs.support++;
				}
			}
			equivalenceClassIBitsets.add(bs);
		}

		return equivalenceClassIBitsets;
	}

	BitSetSupport formPrefixBitsetFromParentDiffsUnion(Set<Integer> parentDiffsUnion) {

		BitSetSupport prefixBitset = new BitSetSupport();
		for(int i=0;i<database.getN();i++) {
			if(!parentDiffsUnion.contains(i)) {
				prefixBitset.bitset.set(i);
				prefixBitset.support++;
			}
		}

		return prefixBitset;
	}
























	private void processEquivalenceClassEclat(Set<Integer> prefixTidset, int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<Set<Integer>> equivalenceClassTidsets) throws IOException {

		int length = prefixLength+1;
		
		if(equivalenceClassItems.size() == 1) {
			
			int item = equivalenceClassItems.get(0);
			Set<Integer> tidset = equivalenceClassTidsets.get(0);
			//save(prefix, prefixLength, itemI, tidsetItemset, support);
			return;
		}

		if(equivalenceClassItems.size() == 2) {
			
			int itemI = equivalenceClassItems.get(0);
			Set<Integer> tidsetI = equivalenceClassTidsets.get(0);
			int supportI = tidsetI.size();
			// save(prefix, prefixLength, itemI, tidsetI, supportI);
			
			int itemJ = equivalenceClassItems.get(1);
			Set<Integer> tidsetJ = equivalenceClassTidsets.get(1);
			int supportJ = tidsetJ.size();
			// save(prefix, prefixLength, itemJ, tidsetJ, supportJ);
			
			Set<Integer> tidsetIJ = this.performINTERSECTION(tidsetI, tidsetJ);
			int supportIJ = tidsetIJ.size();
			
			if(supportIJ >= minSupRelative) {
				int newPrefixLength = prefixLength+1;
				prefix[prefixLength] = itemI;
				//save(prefix, newPrefixLength, itemJ, tidsetIJ, supportIJ);
			}
			return;
		}


		for(int i=0; i< equivalenceClassItems.size(); i++) {
			
			int ETotal = 0;
			int DTotal = 0;

			int suffixI = equivalenceClassItems.get(i);
			Set<Integer> tidsetI = equivalenceClassTidsets.get(i);
			int supportI = tidsetI.size();
			// save(prefix, prefixLength, suffixI, tidsetI, supportI);
			
			List<Integer> equivalenceClassISuffixItems= new ArrayList<Integer>();
			List<Set<Integer>> equivalenceClassITidsets = new ArrayList<Set<Integer>>();

			for(int j=i+1; j < equivalenceClassItems.size(); j++) {
				
				int suffixJ = equivalenceClassItems.get(j);
				Set<Integer> tidsetJ = equivalenceClassTidsets.get(j);
				int supportJ = tidsetJ.size();
				
				Set<Integer> tidsetIJ = this.performINTERSECTION(tidsetI, tidsetJ);
				int supportIJ = tidsetIJ.size();
					
				if(supportIJ >= minSupRelative) {
					ETotal += supportIJ;
					DTotal += (supportI - supportIJ);
					equivalenceClassISuffixItems.add(suffixJ);
					equivalenceClassITidsets.add(tidsetIJ);
				}
			}
			
			if(equivalenceClassISuffixItems.size() > 0) {

				prefix[prefixLength] = suffixI;
				int newPrefixLength = prefixLength+1;
				
				// printing in TIDSETS. Unwanted peice of code ----------------------------------------------------------
								
				// System.out.println("\nfrequent "+(newPrefixLength+1)+"-itemset : \n");
				// for(int p=0;p<equivalenceClassISuffixItems.size();p++) {
				// 	for(int k=0;k<newPrefixLength;k++) {
				// 		System.out.print(prefix[k]);
				// 	}
				// 	System.out.println(","+equivalenceClassISuffixItems.get(p) +" : "+equivalenceClassITidsets.get(p));
				// }

				// ------------------------------------------------------------------------------------------------------

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
					if(EAvg > PointOfDiff){
						
						System.out.println("DECLAT");
						
						List<Set<Integer>> equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
						Set<Integer> parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);
						processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
					else {
						
						System.out.println("ECLAT");
						
						this.processEquivalenceClassEclat(tidsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
				}
				else {
					if(EAvg <= ECLATthreshold){
						
						System.out.println("ECLAT");
						
						this.processEquivalenceClassEclat(tidsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
					else if(EAvg <= DECLATthreshold){
						
						System.out.println("VIPER");
						
						BitSetSupport prefixBitset = formPrefixBitsetFromPrefixTidset(tidsetI);
						List<BitSetSupport> equivalenceClassIBitsets = convertTIDSETStoBITSETS(equivalenceClassITidsets);
						this.processEquivalenceClassViper(prefixBitset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
					}
					else{
						
						System.out.println("DECLAT");
						
						List<Set<Integer>> equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
						Set<Integer> parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);
						processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
				}

				// processEquivalenceClassEclat(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				
			}
		}
	}


	// diff(PXY) = diff(PY) - DIFF(PX)	
	Set<Integer> performINTERSECTION(Set<Integer> tidsetI, Set<Integer> tidsetJ) {
		
		Set<Integer> tidsetIJ = new HashSet<Integer>();
		for(Integer tid : tidsetJ) 
			if(tidsetI.contains(tid))
				tidsetIJ.add(tid);

		return tidsetIJ;
	}

	List<Set<Integer>> convertTIDSETStoDIFFSETS(Set<Integer> prefixTidset, List<Set<Integer>> equivalenceClassITidsets) {
		
		List<Set<Integer>> equivalenceClassIDiffsets = new ArrayList<Set<Integer>>();
		
		for(Set<Integer> tidset : equivalenceClassITidsets) {
			Set<Integer> diffset = new HashSet<Integer>();
			for(int tid : prefixTidset)
				if(!tidset.contains(tid))
					diffset.add(tid);
			equivalenceClassIDiffsets.add(diffset);
		}

		return equivalenceClassIDiffsets;
	}

	Set<Integer> formParentDiffsUnionFromPrefixTidset(Set<Integer> prefixTidset) {

		Set<Integer> parentDiffsUnion = new HashSet<Integer>();
		for(int i=0;i<database.getN();i++) {
			if(!prefixTidset.contains(i))
				parentDiffsUnion.add(i);
		}

		return parentDiffsUnion;
	}

	List<BitSetSupport> convertTIDSETStoBITSETS(List<Set<Integer>> equivalenceClassITidsets) {
		
		List<BitSetSupport> equivalenceClassIBitsets = new ArrayList<BitSetSupport>();

		for(Set<Integer> tidset : equivalenceClassITidsets) {
			BitSetSupport bs = new BitSetSupport();
			for(Integer tid : tidset) {
				bs.bitset.set(tid);
				bs.support++;
			}
			equivalenceClassIBitsets.add(bs);
		}

		return equivalenceClassIBitsets;
	}

	BitSetSupport formPrefixBitsetFromPrefixTidset(Set<Integer> prefixTidset) {
		
		BitSetSupport prefixBitset = new BitSetSupport();

		for(Integer tid : prefixTidset) {
			prefixBitset.bitset.set(tid);
			prefixBitset.support++;
		}

		return prefixBitset;
	}













	private void processEquivalenceClassViper(BitSetSupport prefixBitset, int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<BitSetSupport> equivalenceClassBitsets) throws IOException {

		int length = prefixLength+1;
		
		if(equivalenceClassItems.size() == 1) {
			
			int item = equivalenceClassItems.get(0);
			BitSetSupport bitset = equivalenceClassBitsets.get(0);
			//save(prefix, prefixLength, itemI, tidsetItemset, support);
			return;
		}

		if(equivalenceClassItems.size() == 2) {
			
			int itemI = equivalenceClassItems.get(0);
			BitSetSupport bitsetI = equivalenceClassBitsets.get(0);
			int supportI = bitsetI.support;
			// save(prefix, prefixLength, itemI, tidsetI, supportI);
			
			int itemJ = equivalenceClassItems.get(1);
			BitSetSupport bitsetJ = equivalenceClassBitsets.get(1);
			int supportJ = bitsetJ.support;
			// save(prefix, prefixLength, itemJ, tidsetJ, supportJ);
			
			BitSetSupport bitsetIJ = this.performAND(bitsetI, bitsetJ);
			int supportIJ = bitsetIJ.support;
			
			if(supportIJ >= minSupRelative) {
				int newPrefixLength = prefixLength+1;
				prefix[prefixLength] = itemI;
				//save(prefix, newPrefixLength, itemJ, tidsetIJ, supportIJ);
			}
			return;
		}

		
		for(int i=0; i< equivalenceClassItems.size(); i++) {
			
			int ETotal = 0;
			int DTotal = 0;

			int suffixI = equivalenceClassItems.get(i);
			BitSetSupport bitsetI = equivalenceClassBitsets.get(i);
			int supportI = bitsetI.support;
			// save(prefix, prefixLength, suffixI, tidsetI, supportI);
			
			List<Integer> equivalenceClassISuffixItems= new ArrayList<Integer>();
			List<BitSetSupport> equivalenceClassIBitsets = new ArrayList<BitSetSupport>();

			for(int j=i+1; j < equivalenceClassItems.size(); j++) {
				
				int suffixJ = equivalenceClassItems.get(j);
				BitSetSupport bitsetJ = equivalenceClassBitsets.get(j);
				int supportJ = bitsetJ.support;
				
				BitSetSupport bitsetIJ = this.performAND(bitsetI, bitsetJ);
				int supportIJ = bitsetIJ.support;
					
				if(supportIJ >= minSupRelative) {
					ETotal += supportIJ;
					DTotal += (supportI - supportIJ);
					equivalenceClassISuffixItems.add(suffixJ);
					equivalenceClassIBitsets.add(bitsetIJ);
				}
			}
			
			if(equivalenceClassISuffixItems.size() > 0) {

				prefix[prefixLength] = suffixI;
				int newPrefixLength = prefixLength+1;
				
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
					if(EAvg > PointOfDiff){
						
						System.out.println("DECLAT");
					
						List<Set<Integer>> equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
						Set<Integer> parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
						processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
					else {
						
						System.out.println("ECLAT");
					
						List<Set<Integer>> equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
						Set<Integer> prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
						processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
				}
				else {
					if(EAvg <= ECLATthreshold){
						
						System.out.println("ECLAT");
						
						List<Set<Integer>> equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
						Set<Integer> prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
						processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
					else if(EAvg <= DECLATthreshold){
						
						System.out.println("VIPER");
						
						this.processEquivalenceClassViper(bitsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassBitsets);
					}
					else{
						
						System.out.println("DECLAT");
						
						List<Set<Integer>> equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
						Set<Integer> parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
						processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
				}

				// processEquivalenceClassEclat(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				
			}
		}

	}

	public class BitSetSupport{
		BitSet bitset = new BitSet();
		int support;
	}

	BitSetSupport performAND(BitSetSupport bitsetI, BitSetSupport bitsetJ) {
		
		BitSetSupport bitsetIJ = new BitSetSupport();
		bitsetIJ.bitset = (BitSet)bitsetI.bitset.clone();
		bitsetIJ.bitset.and(bitsetJ.bitset);
		bitsetIJ.support = bitsetIJ.bitset.cardinality();

		return bitsetIJ;
	}

	List<Set<Integer>> convertBITSETStoTIDSETS(List<BitSetSupport> equivalenceClassIBitsets) {
		return null;
	}

	Set<Integer> formPrefixTidsetFromPrefixBitsets(BitSetSupport prefixBitset) {
		return null;
	}

	List<Set<Integer>> convertBITSETStoDIFFSETS(BitSetSupport prefixBitset, List<BitSetSupport> equivalenceClassIBitsets) {
		return null;
	}

	Set<Integer> formParentDiffsUnionFromPrefixBitset(BitSetSupport prefixBitset) {
		return null;
	}

}
