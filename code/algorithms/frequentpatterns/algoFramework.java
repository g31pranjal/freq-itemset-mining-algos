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
import code.tools.MemoryLogger;


public class algoFramework {

	protected int minSupRelative;
	protected TransactionDatabase database;
	protected Map<Integer, Set<Integer>> verticalDB;
	BufferedWriter writer = null; 
	protected Itemsets frequentItemsets;
	protected int itemsetCount;
	protected long startTime;
	protected long endTime;
	protected int algo;
	protected int[] rec;
	protected int getN;

	protected int INTSIZE = 32; //bits

	// 1 : ECLAT, 2 : VIPER, 3 : DECLAT, 0 : VARIABLE

	// the default route of algo is 0 i.e. VARIABLE
	public algoFramework() {
		this.algo = 0;
		rec = new int[4];
	}

	public algoFramework(int algo) {
		this.algo = algo;
		rec = new int[4];
	}

	public void runAlgo(String outputFile, TransactionDatabase database, double minsupp) throws IOException {

		MemoryLogger.getInstance().reset();

		startTime = System.currentTimeMillis();

		this.database = database;
		this.getN = database.getN();
		this.minSupRelative = (int) Math.ceil(minsupp * getN);
		this.verticalDB = database.getVerticalDB();

		writer = new BufferedWriter(new FileWriter(outputFile));
		
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

			Integer ECLATthreshold  = (int)(getN*(1.0/INTSIZE));
			Integer DECLATthreshold = (int)(getN*(31.0/INTSIZE));

			System.out.println("\nECLAT threshold : " + ECLATthreshold + ", DECLAT threshold : " + DECLATthreshold);
			System.out.println("Average value : " + avgTidsetSize);

			if(algo == 0) {
				if(avgTidsetSize <= ECLATthreshold ) {
					// System.out.println("ECLAT");
					this.constructTIDSETS(oneFrequentItems);
				}
				else if(avgTidsetSize <= DECLATthreshold){
					// System.out.println("VIPER");
					this.constructBITSETS(oneFrequentItems);
				}
				else{
					// System.out.println("DECLAT");
					this.constructDIFFSETS(oneFrequentItems);
				}
			}
			else {
				if(algo == 1) {
					this.constructTIDSETS(oneFrequentItems);
				}
				else if(algo == 2){
					this.constructBITSETS(oneFrequentItems);
				}
				else if(algo == 3){
					this.constructDIFFSETS(oneFrequentItems);
				}	
			}
		}

		endTime = System.currentTimeMillis();
		writer.close();

		MemoryLogger.getInstance().checkMemory();
		this.printStats();


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
		for(int i=0;i<getN;i++) 
			enot.add(i);

		this.database = null;
		this.verticalDB = null;

		this.processEquivalenceClassEclat(enot ,new int[1000], 0, getN, equivalenceClassItems, equivalenceClassTidsets);
	}


	public void constructBITSETS(List<Integer> equivalenceClassItems) throws IOException {
		
		List<BitSetSupport> equivalenceClassBitsets = new ArrayList<BitSetSupport>();
	
		// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
		for(Integer item : equivalenceClassItems) {
			
			BitSetSupport bs = new BitSetSupport();
			for(Integer i : verticalDB.get(item)) {
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
		for(int i=0;i<getN;i++) {
			enot.bitset.set(i);
			enot.support++;
		}

		this.database = null;
		this.verticalDB = null;

		this.processEquivalenceClassViper(enot, new int[1000], 0, getN, equivalenceClassItems, equivalenceClassBitsets);
	}


	public void constructDIFFSETS(List<Integer> equivalenceClassItems) throws IOException{
		
		List<Set<Integer> > equivalenceClassDiffsets = new ArrayList<Set<Integer>>();

		// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
		for(Integer item : equivalenceClassItems) {
			Set<Integer> tidset = verticalDB.get(item);
			Set<Integer> diffset = new HashSet<Integer>();
			for(int i=0;i<getN;i++) {
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

		this.database = null;
		this.verticalDB = null;

		this.processEquivalenceClassDEclat(new HashSet<Integer>(), new int[1000], 0, getN, equivalenceClassItems, equivalenceClassDiffsets);
	}


	private void processEquivalenceClassEclat(Set<Integer> prefixTidset, int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<Set<Integer>> equivalenceClassTidsets) throws IOException {


		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>> ECLAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
		System.out.print("Prefix : ");
		for(int i=0;i<prefixLength;i++)
			System.out.print(prefix[i] + ", ");
		System.out.println();
		MemoryLogger.getInstance().checkMemory();

		rec[1]++;

		int length = prefixLength+1;
		
		if(equivalenceClassItems.size() == 1) {
			
			int item = equivalenceClassItems.get(0);
			Set<Integer> tidset = equivalenceClassTidsets.get(0);
			int support = tidset.size();
			this.save(prefix, prefixLength, item, support);
			return;
		}

		if(equivalenceClassItems.size() == 2) {
			
			int itemI = equivalenceClassItems.get(0);
			Set<Integer> tidsetI = equivalenceClassTidsets.get(0);
			int supportI = tidsetI.size();
			this.save(prefix, prefixLength, itemI, supportI);
			
			int itemJ = equivalenceClassItems.get(1);
			Set<Integer> tidsetJ = equivalenceClassTidsets.get(1);
			int supportJ = tidsetJ.size();
			this.save(prefix, prefixLength, itemJ, supportJ);
			
			Set<Integer> tidsetIJ = this.performINTERSECTION(tidsetI, tidsetJ);
			int supportIJ = tidsetIJ.size();
			
			if(supportIJ >= minSupRelative) {
				int newPrefixLength = prefixLength+1;
				prefix[prefixLength] = itemI;
				this.save(prefix, newPrefixLength, itemJ, supportIJ);
			}
			return;
		}


		for(int i=0; i< equivalenceClassItems.size(); i++) {
			
			int ETotal = 0;
			int DTotal = 0;

			int suffixI = equivalenceClassItems.get(i);
			Set<Integer> tidsetI = equivalenceClassTidsets.get(i);
			int supportI = tidsetI.size();
			this.save(prefix, prefixLength, suffixI, supportI);
			
			List<Integer> equivalenceClassISuffixItems= new ArrayList<Integer>();
			List<Set<Integer>> equivalenceClassITidsets = new ArrayList<Set<Integer>>();

			for(int j=i+1; j < equivalenceClassItems.size(); j++) {
				
				// System.out.println(".....CP2");

				int suffixJ = equivalenceClassItems.get(j);
				Set<Integer> tidsetJ = equivalenceClassTidsets.get(j);
				int supportJ = tidsetJ.size();
				
				Set<Integer> tidsetIJ = this.performINTERSECTION(tidsetI, tidsetJ);
				int supportIJ = tidsetIJ.size();
				
				// System.out.println(".....CP3");

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

				if(algo == 1) {
					
					this.processEquivalenceClassEclat(tidsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);

				}
				else {

					double DAvg = DTotal / (double)equivalenceClassISuffixItems.size();
					double EAvg = ETotal / (double)equivalenceClassISuffixItems.size();
					
					int ECLATthreshold = (int)(getN*(1.0/INTSIZE) );
					int ECLATstart = 0;
					int DECLATthreshold  = supportI - ECLATthreshold;
					int DECLATstart  = supportI;

					// System.out.println("ECLAT thr. : "+ECLATthreshold+" , "+ "DECLAT thr. : "+DECLATthreshold);
					// System.out.println("ECLAT str. : "+ECLATstart+" , "+ "DECLAT str. : "+DECLATstart);
					// System.out.println("E(avg) : "+EAvg);
					// System.out.println("D(avg) : "+DAvg);

					if(DECLATthreshold <= ECLATthreshold) {
						double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
						// System.out.println("POINT OF DIFF : " + PointOfDiff);
						if(EAvg > PointOfDiff){
							
							// System.out.println("DECLAT");
							
							List<Set<Integer>> equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
							Set<Integer> parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);
							processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
							
						}
						else {
							
							// System.out.println("ECLAT");
							
							this.processEquivalenceClassEclat(tidsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
						}
					}
					else {
						if(EAvg <= ECLATthreshold){
							
							// System.out.println("ECLAT");
							
							this.processEquivalenceClassEclat(tidsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
						}
						else if(EAvg <= DECLATthreshold){
							
							// System.out.println("VIPER");
							
							BitSetSupport prefixBitset = formPrefixBitsetFromPrefixTidset(tidsetI);
							List<BitSetSupport> equivalenceClassIBitsets = convertTIDSETStoBITSETS(equivalenceClassITidsets);
							
							this.processEquivalenceClassViper(prefixBitset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
						}
						else{
							
							// System.out.println("DECLAT");
						
							List<Set<Integer>> equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
							Set<Integer> parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);
							processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
						}
					}
				}
				
			}
		}

		MemoryLogger.getInstance().checkMemory();

	}


	Set<Integer> performINTERSECTION(Set<Integer> tidsetI, Set<Integer> tidsetJ) {
		
		Set<Integer> tidsetIJ = new HashSet<Integer>();
		if(tidsetJ.size() < tidsetI.size()){
			for(Integer tid : tidsetJ) 
				if(tidsetI.contains(tid))
					tidsetIJ.add(tid);
		}
		else {
			for(Integer tid : tidsetI) 
				if(tidsetJ.contains(tid))
					tidsetIJ.add(tid);
		}
	
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
		for(int i=0;i<getN;i++) {
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


		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>> VIPER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
		System.out.print("Prefix : ");
		for(int i=0;i<prefixLength;i++)
			System.out.print(prefix[i] + ", ");
		System.out.println();
		MemoryLogger.getInstance().checkMemory();

		rec[2]++;
		
		int length = prefixLength+1;
		
		if(equivalenceClassItems.size() == 1) {
			
			int item = equivalenceClassItems.get(0);
			BitSetSupport bitset = equivalenceClassBitsets.get(0);
			save(prefix, prefixLength, item, bitset.support);
			return;
		}

		if(equivalenceClassItems.size() == 2) {
			
			int itemI = equivalenceClassItems.get(0);
			BitSetSupport bitsetI = equivalenceClassBitsets.get(0);
			int supportI = bitsetI.support;
			save(prefix, prefixLength, itemI, supportI);
			
			int itemJ = equivalenceClassItems.get(1);
			BitSetSupport bitsetJ = equivalenceClassBitsets.get(1);
			int supportJ = bitsetJ.support;
			save(prefix, prefixLength, itemJ, supportJ);
			
			BitSetSupport bitsetIJ = this.performAND(bitsetI, bitsetJ);
			int supportIJ = bitsetIJ.support;
			
			if(supportIJ >= minSupRelative) {
				int newPrefixLength = prefixLength+1;
				prefix[prefixLength] = itemI;
				save(prefix, newPrefixLength, itemJ, supportIJ);
			}
			return;
		}

		
		for(int i=0; i< equivalenceClassItems.size(); i++) {
			
			int ETotal = 0;
			int DTotal = 0;

			int suffixI = equivalenceClassItems.get(i);
			BitSetSupport bitsetI = equivalenceClassBitsets.get(i);
			int supportI = bitsetI.support;
			this.save(prefix, prefixLength, suffixI, supportI);
			
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
				
				if(algo == 2) {

					this.processEquivalenceClassViper(bitsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);

				}
				else {

					double DAvg = DTotal / (double)equivalenceClassISuffixItems.size();
					double EAvg = ETotal / (double)equivalenceClassISuffixItems.size();
					
					int ECLATthreshold = (int)(getN*(1.0/INTSIZE) );
					int ECLATstart = 0;
					int DECLATthreshold  = supportI - ECLATthreshold;
					int DECLATstart  = supportI;

					// System.out.println("ECLAT thr. : "+ECLATthreshold+" , "+ "DECLAT thr. : "+DECLATthreshold);
					// System.out.println("ECLAT str. : "+ECLATstart+" , "+ "DECLAT str. : "+DECLATstart);
					// System.out.println("E(avg) : "+EAvg);
					// System.out.println("D(avg) : "+DAvg);

					if(DECLATthreshold <= ECLATthreshold) {
						double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
						// System.out.println("POINT OF DIFF : " + PointOfDiff);
						if(EAvg > PointOfDiff){
							
							// System.out.println("DECLAT");
						
							List<Set<Integer>> equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
							Set<Integer> parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
							processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);

							// this.processEquivalenceClassViper(bitsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);

						}
						else {
							
							// System.out.println("ECLAT");
						
							List<Set<Integer>> equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
							Set<Integer> prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
							processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
						}
					}
					else {
						if(EAvg <= ECLATthreshold){
							
							// System.out.println("ECLAT");
							
							List<Set<Integer>> equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
							Set<Integer> prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
							processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
						}
						else if(EAvg <= DECLATthreshold){
							
							// System.out.println("VIPER");
							
							this.processEquivalenceClassViper(bitsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
						}
						else{
							
							// System.out.println("DECLAT");
							
							List<Set<Integer>> equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
							Set<Integer> parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
							processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
							
							// this.processEquivalenceClassViper(bitsetI, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);

						}
					}

				}
				
			}
		}

		MemoryLogger.getInstance().checkMemory();

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
		
		List<Set<Integer>> equivalenceClassITidsets = new ArrayList<Set<Integer>>();

		for( BitSetSupport bitset : equivalenceClassIBitsets ) {
			Set<Integer> tidset = new HashSet<Integer>();
			for(int i=bitset.bitset.nextSetBit(0); i >= 0; i = bitset.bitset.nextSetBit(i+1)) {
				tidset.add(i);
			}
			equivalenceClassITidsets.add(tidset);
		}
		return equivalenceClassITidsets;
	}


	Set<Integer> formPrefixTidsetFromPrefixBitsets(BitSetSupport prefixBitset) {
		Set<Integer> prefixTidset = new HashSet<Integer>();
		for(int i = prefixBitset.bitset.nextSetBit(0); i >= 0; i = prefixBitset.bitset.nextSetBit(i+1)) {
			prefixTidset.add(i);
		}
		return prefixTidset;
	}


	List<Set<Integer>> convertBITSETStoDIFFSETS(BitSetSupport prefixBitset, List<BitSetSupport> equivalenceClassIBitsets) {
		
		List<Set<Integer>> equivalenceClassIDiffsets = new ArrayList<Set<Integer>>();
		
		for(BitSetSupport bitset : equivalenceClassIBitsets) {
			Set<Integer> diffset = new HashSet<Integer>();
			for(int i = prefixBitset.bitset.nextSetBit(0); i>= 0; i = prefixBitset.bitset.nextSetBit(i+1))
				if(!bitset.bitset.get(i))
					diffset.add(i);
			equivalenceClassIDiffsets.add(diffset);
		}

		return equivalenceClassIDiffsets;
	}


	Set<Integer> formParentDiffsUnionFromPrefixBitset(BitSetSupport prefixBitset) {
	
		Set<Integer> parentDiffsUnion = new HashSet<Integer>();
		for(int i=0;i<getN;i++) {
			if(!prefixBitset.bitset.get(i))
				parentDiffsUnion.add(i);
		}

		return parentDiffsUnion;
	}


	private void processEquivalenceClassDEclat(Set<Integer> parentDiffsUnion, int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<Set<Integer>> equivalenceClassDiffsets) throws IOException {


		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>> DECLAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
		System.out.print("Prefix : ");
		for(int i=0;i<prefixLength;i++)
			System.out.print(prefix[i] + ", ");
		System.out.println();
		MemoryLogger.getInstance().checkMemory();

		rec[3]++;
		
		int length = prefixLength+1;
		
		if(equivalenceClassItems.size() == 1) {
			
			int item = equivalenceClassItems.get(0);
			Set<Integer> diffset = equivalenceClassDiffsets.get(0);
			int support = prefixSupport = diffset.size();
			save(prefix, prefixLength, item, support);
			return;
		}

		if(equivalenceClassItems.size() == 2) {
			
			int itemI = equivalenceClassItems.get(0);
			Set<Integer> diffsetI = equivalenceClassDiffsets.get(0);
			int supportI = prefixSupport - diffsetI.size();
			save(prefix, prefixLength, itemI, supportI);
			
			int itemJ = equivalenceClassItems.get(1);
			Set<Integer> diffsetJ = equivalenceClassDiffsets.get(1);
			int supportJ = prefixSupport - diffsetJ.size();
			save(prefix, prefixLength, itemJ, supportJ);
			
			Set<Integer> diffsetIJ = this.performDIFFERENCE(diffsetI, diffsetJ);
			int supportIJ = supportI - diffsetIJ.size();
			
			if(supportIJ >= minSupRelative) {
				int newPrefixLength = prefixLength+1;
				prefix[prefixLength] = itemI;
				save(prefix, newPrefixLength, itemJ, supportIJ);
			
			}
			return;
		}

		for(int i=0; i< equivalenceClassItems.size(); i++) {
			
			int ETotal = 0;
			int DTotal = 0;

			int suffixI = equivalenceClassItems.get(i);
			Set<Integer> diffsetI = equivalenceClassDiffsets.get(i);
			int supportI = prefixSupport - diffsetI.size();
			save(prefix, prefixLength, suffixI, supportI);
			
			List<Integer> equivalenceClassISuffixItems= new ArrayList<Integer>();
			List<Set<Integer>> equivalenceClassIDiffsets = new ArrayList<Set<Integer>>();

			Set<Integer> newParentDiffsUnion = new HashSet<Integer>(parentDiffsUnion);
			newParentDiffsUnion.addAll(diffsetI);

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

				if(algo == 1) {

					this.processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);

				}
				else {

					double DAvg = DTotal / (double)equivalenceClassISuffixItems.size();
					double EAvg = ETotal / (double)equivalenceClassISuffixItems.size();
					
					int ECLATthreshold = (int)(getN*(1.0/INTSIZE) );
					int ECLATstart = 0;
					int DECLATthreshold  = supportI - ECLATthreshold;
					int DECLATstart  = supportI;

					// System.out.println("ECLAT thr. : "+ECLATthreshold+" , "+ "DECLAT thr. : "+DECLATthreshold);
					// System.out.println("ECLAT str. : "+ECLATstart+" , "+ "DECLAT str. : "+DECLATstart);
					// System.out.println("E(avg) : "+EAvg);
					// System.out.println("D(avg) : "+DAvg);

					if(DECLATthreshold <= ECLATthreshold) {
						double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
						// System.out.println("POINT OF DIFF : " + PointOfDiff);
						if(EAvg > PointOfDiff){
							
							// System.out.println("DECLAT");
							
							this.processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
						}
						else {
							
							// System.out.println("ECLAT");

							List<Set<Integer>> equivalenceClassITidsets = convertDIFFSETStoTIDSETS(newParentDiffsUnion, equivalenceClassIDiffsets);
							Set<Integer> prefixTidset = formPrefixTidsetFromParentDiffsUnion(newParentDiffsUnion);
							this.processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
						}
					}
					else {
						if(EAvg <= ECLATthreshold){
							
							// System.out.println("ECLAT");
							
							List<Set<Integer>> equivalenceClassITidsets = convertDIFFSETStoTIDSETS(newParentDiffsUnion, equivalenceClassIDiffsets);
							Set<Integer> prefixTidset = formPrefixTidsetFromParentDiffsUnion(newParentDiffsUnion);
							this.processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
						}
						else if(EAvg <= DECLATthreshold){
							
							// System.out.println("VIPER");
							
							List<BitSetSupport> equivalenceClassIBitsets = convertDIFFSETStoBITSETS(newParentDiffsUnion, equivalenceClassIDiffsets);
							BitSetSupport prefixBitset = formPrefixBitsetFromParentDiffsUnion(newParentDiffsUnion);
							this.processEquivalenceClassViper(prefixBitset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
						}
						else{
							
							// System.out.println("DECLAT");
							
							this.processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
						}
					}

				}
			}
		}
		
		MemoryLogger.getInstance().checkMemory();

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
			for(int i=0;i<getN;i++) {
				if(!parentDiffsUnion.contains(i) && !diffset.contains(i))
					tid.add(i);
			}
			equivalenceClassITidsets.add(tid);
		}

		return equivalenceClassITidsets;
	}


	Set<Integer> formPrefixTidsetFromParentDiffsUnion(Set<Integer> parentDiffsUnion) {

		Set<Integer> prefixTidset = new HashSet<Integer>();
		for(int i=0;i<getN;i++) {
			if(!parentDiffsUnion.contains(i))
				prefixTidset.add(i);
		}

		return prefixTidset;
	}


	List<BitSetSupport> convertDIFFSETStoBITSETS(Set<Integer> parentDiffsUnion, List<Set<Integer>> equivalenceClassIDiffsets) {

		List<BitSetSupport> equivalenceClassIBitsets = new ArrayList<BitSetSupport>();

		for(Set<Integer> diffset : equivalenceClassIDiffsets) {
			BitSetSupport bs = new BitSetSupport();
			for(int i=0;i<getN;i++) {
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
		for(int i=0;i<getN;i++) {
			if(!parentDiffsUnion.contains(i)) {
				prefixBitset.bitset.set(i);
				prefixBitset.support++;
			}
		}

		return prefixBitset;
	}


	public void printStats() {
		long temps = endTime - startTime;
		System.out.println("\n\n===================================================");
		System.out.println(" Transactions count from database : " + getN);
		System.out.println(" Total time ~ " + temps + " ms");
		System.out.println(" Maximum memory usage : "+ MemoryLogger.getInstance().getMaxMemory() + " mb");
		System.out.println(" Usage : ECLAT " +rec[1]+" VIPER "+rec[2]+" DECLAT "+rec[3]);
		System.out.println("===================================================");
	}

	private void save(int[] prefix, int prefixLength, int suffixItem, int support) throws IOException {
		
		itemsetCount++;
	
		StringBuilder buffer = new StringBuilder();
		for(int i=0; i < prefixLength; i++) {
			int item = prefix[i];
			buffer.append(item);
			buffer.append(" ");
		}
		buffer.append(suffixItem);
		
		buffer.append(" #SUP: ");
		buffer.append(support);
		
		writer.write(buffer.toString());
		writer.newLine();

	}

}
