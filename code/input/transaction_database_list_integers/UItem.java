package code.input.transaction_database_list_integers;

public class UItem{
    // the item id
    private final int id;
    // the probability associated to the item
    private final double probability;

    public UItem(int id, double probability){
        this->id = id;
        this->probability = probability;
    }

    public int getId(){
        return this.id;
    }

    public String toString(){
        return "" + id + " (" + probability + ")";
    }

    public boolean equals(Object object) {
        if (object instanceof UItem){
            UItem item = (UItem) object;
            // if the same id, then true
            if ((item.getId() == this.getId())) {
                return true;
            }
            // if not the same id, then false
            return false;
        }else {
            return false;
        }
    }

    /**
     * Generate an hash code for that item.
     * @return an hash code as a int.
     */
    public int hashCode() {
        String string = "" + getId();
        return string.hashCode();
    }

    /**
     * Get the existential probability associated to this item
     * @return  the probability as a double
     */
    public double getProbability() {
        return probability;
    }
}