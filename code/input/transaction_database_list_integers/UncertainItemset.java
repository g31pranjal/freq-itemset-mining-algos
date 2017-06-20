package code.input.transaction_database_list_integers;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;


public class UncertainItemset{
    private List<UItem> items = new ArrayList<UItem>();
    private double expectedsupport = 0;

    public UncertainItemset(){

    }

    public double getExpectedSupport() {
        return expectedsupport;
    }

    /**
     * Get the expected support as a five decimals string
     * @return a string
     */
    public String getSupportAsString() {
        DecimalFormat format = new DecimalFormat();
        format.setMinimumFractionDigits(0);
        format.setMaximumFractionDigits(5);
        return format.format(expectedsupport);
    }

    /**
     * Increase the expected support of this itemset by a given amount.
     * @param supp the amount of support.
     */
    void increaseSupportBy(double supp) {
        expectedsupport += supp;
    }

    void addItem(UItem value){
        items.add(value);
    }

    public List<UItem> getItems(){
        return items;
    }

    public UItem get(int index){
        return items.get(index);
    }

    public void print(){
        System.out.print(toString());
    }

    /**
     * Print the items in this itemset to System.out.
     */
    public void printWithoutSupport(){
        StringBuilder r = new StringBuilder ();
        for(UItem attribute : items){
            r.append(attribute.getId());
            r.append(' ');
        }
        System.out.print(r);
    }

    public String toString(){
        StringBuilder r = new StringBuilder ();
        for(UItem attribute : items){
            r.append(attribute.toString());
            r.append(' ');
        }
        return r.toString();
    }

    public boolean contains(UItem item){
        return items.contains(item);
    }

    public boolean isLexicallySmallerthan(UItem itemset2){
        // for each item in this itemset
        for(int i=0; i< items.size(); i++){
            // if it is larger than the item at the same position in itemset2
            // return false
            if(items.get(i).getId() > itemset2.items.get(i).getId()){
                return false;
            }
            // if it is smaller than the item at the same position in itemset2
            // return true
            else if(items.get(i).getId() < itemset2.items.get(i).getId()){
                return true;
            }
        }
        // otherwise return true
        return true;
    }

    public boolean isEqualTo(UItem itemset2){
        // if not the same size, they can't be equal!
        if(items.size() != itemset2.items.size()){
            return false;
        }
        // for each item
        for(UItem val : items){
            // check if it is contained in the other itemset
            // if not they are not equal.
            if(!itemset2.contains(val)){
                return false;
            }
        }
        // they are equal, then return true
        return true;
    }

    void setExpectedSupport(double expectedsupport) {
        this.expectedsupport = expectedsupport;
    }

    public UncertainItemset cloneItemSetMinusOneItem(UItem itemToExclude){
        // create a new itemset
        UncertainItemset itemset = new UncertainItemset();
        // for each item
        for(UItem item : items){
            // if it is not the one to be excluded, then add it
            if(!item.equals(itemToExclude)){
                itemset.addItem(item);
            }
        }
        // return the itemset
        return itemset;
    }

    public int size(){
        return items.size();
    }

    public UItem allTheSameExceptLastItem(UncertainItemset itemset2) {
        // if not the same size, then return null
        if(itemset2.size() != items.size()){
            return null;
        }
        for(int i=0; i< items.size(); i++){
            // if they are the last items
            if(i == items.size()-1){
                // the one from items should be smaller (lexical order) and different than the one of itemset2
                if(items.get(i).getId() >= itemset2.get(i).getId()){
                    return null;
                }
            }
            // if they are not the last items, they  should be different
            else if(items.get(i).getId() != itemset2.get(i).getId()){
                return null;
            }
        }
        return itemset2.get(itemset2.size()-1);
    }

    /**
     * Set the items in this itemsets.
     * @param items a list of items.
     */
    void setItems(List<UItem> items) {
        this.items = items;
    }

}