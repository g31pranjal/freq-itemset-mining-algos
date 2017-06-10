package code.algorithms.frequentpatterns.eclat;

import java.util.HashSet;
import java.util.Set;

import code.input.transaction_database_list_integers.TransactionDatabase;
import code.patterns.itemset_array_integers_with_count.Itemset;
import code.patterns.itemset_array_integers_with_count.Itemsets;
import code.tools.MemoryLogger;
 

public class AlgoDEclat extends AlgoEclat{
	
		public void printStats() {
		System.out.println("=============  dECLAT v0.96r18 - STATS =============");
		long temps = endTime - startTimestamp;
		System.out.println(" Transactions count from database : "
				+ database.size());
		System.out.println(" Frequent itemsets count : "
				+ itemsetCount);
		System.out.println(" Total time ~ " + temps + " ms");
		System.out.println(" Maximum memory usage : "
				+ MemoryLogger.getInstance().getMaxMemory() + " mb");
		System.out.println("===================================================");
	}
	
	 Set<Integer> performANDFirstTime(Set<Integer> tidsetI, int supportI,
			Set<Integer> tidsetJ, int supportJ) {
		// Create the new tidset that will store the difference
		Set<Integer> diffsetIJ = new HashSet<Integer>();
		// for each tid containing j
		for(Integer tid : tidsetI) {
			// if the transaction does not contain i, add it to the diffset
			if(tidsetJ.contains(tid) == false) {
				// add it to the intersection
				diffsetIJ.add(tid);
			}			
		}
		// return the new tidset
		return diffsetIJ;
	}

	 Set<Integer> performAND(Set<Integer> tidsetI, int supportI,
			Set<Integer> tidsetJ, int supportJ) {
		// Create the new tidset that will store the difference
		Set<Integer> diffsetIJ = new HashSet<Integer>();
		// for each tid containing j
		for(Integer tid : tidsetJ) {
			// if the transaction does not contain i, add it to the diffset
			if(tidsetI.contains(tid) == false) {
				// add it to the intersection
				diffsetIJ.add(tid);
			}			
		}
		// return the new tidset
		return diffsetIJ;
	}
	
	int calculateSupport(int lengthOfX, int supportPrefix,  Set<Integer> tidsetX) {
		// if length of prefix = 1 then we are using tidsets
		if(lengthOfX == 1) {
			return tidsetX.size();
		}else {
			// otherwise we are using diffsets
			return supportPrefix - tidsetX.size();
		}
	}

}
