
import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._


object algoFramework {

	def main(args : Array[String]) {

		val conf = new SparkConf().setMaster("local").setAppName("My App")
		val sc = new SparkContext(conf)
		
		val i1 = sc.textFile("umdblab/datasets/kddcup99.txt")
		val i2 = i1.zipWithIndex()
		val i3 = i2.flatMap( a => a._1.split(" ").map(word => (word, a._2) )  )
		val i4 = i3.groupByKey()

		i4.saveAsTextFile("out")

		
		

	}

}



// val input = sc.textFile(inputFile)
// val mapi  = input.flatMap()


// val words = input.flatMap(line => line.split(" "))
// val counts = words.map(word => (word, 1)).reduceByKey{case (x, y) => x + y}
// counts.saveAsTextFile(outputFile)