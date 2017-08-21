import java.io.File

import org.apache.spark.SparkContext
import org.apache.spark.SparkConf
import org.apache.spark.rdd.RDD


object AlterDB {
  
  def main(args: Array[String]) {
    
    val appName = "Umdblab Project"
    val masterUrl = "local[4]"
    val filename ="file://" + new File("").getAbsolutePath + "/src/resources/test1.txt"
    
    val conf = new SparkConf()
      .setAppName(appName)
      .setMaster(masterUrl)
    val sc = new SparkContext(conf)
    
    
    readFile(filename, sc)
    
  }
  
  def readFile(filepath: String, sc: SparkContext): Unit = {
    
    val distFile = sc.textFile(filepath)
    
    distFile.foreach(x => println(x))
    
  }
  
}

