package umdblab_scala

import java.io.File

import org.apache.spark.SparkContext
import org.apache.spark.SparkConf
import org.apache.spark.rdd.RDD


object AlterDB {
  
  val appName = "Umdblab Project Scala Part"
  val masterUrl = "local[4]"
  val filename ="file://" + new File("").getAbsolutePath + "/src/resources/test1.txt"
  
  val conf = new SparkConf()
    .setAppName(appName)
    .setMaster(masterUrl)
  val sc = new SparkContext(conf)
  
  def main(args: Array[String]) {
    //construct horizontal DB and vertical DB
    val transactionDB =  new TransactionDB()
    transactionDB.loadFile(filename)
    
  }
  
}

