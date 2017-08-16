import org.apache.spark.sql.SparkSession


object AlterDB {
  
  def main(args: Array[String]) {
    
    val logFile = "/home/zhida/Downloads/spark-2.2.0-bin-hadoop2.7/README.md" // Should be some file on your system
    val spark = SparkSession.builder.appName("AlterDB Application").getOrCreate()
    val logData = spark.read.textFile(logFile).cache()
    val numAs = logData.filter(line => line.contains("a")).count()
    val numBs = logData.filter(line => line.contains("b")).count()
    println(s"Lines with a: $numAs, Lines with b: $numBs")
    
    spark.stop()
    
  }
  
}

