package code.input.transaction_database_list_integers;

public class TidAndProb{
    private int tid;
    private double probability;

    public TidAndProb (int tid, double probability){
        this.tid = tid;
        this.probability = probability;
    }

    public int getTid(){
        return this.tid;
    }

    public double getProbability(){
        return this.probability;
    }

    public String toString(){
        return "" + tid + " (" + probability + ")";
    }

    public int hashCode() {
        String string = "" + tid;
        return string.hashCode();
    }
}