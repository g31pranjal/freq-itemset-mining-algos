package umdblab_scala

import org.apache.spark.rdd.RDD
import AlterDB.sc

class TransactionDB extends java.io.Serializable {
  
  var _items: Set[Int] = Set[Int]()
  var _horizontalDB: Vector[Vector[Int]] = Vector[Vector[Int]]()
  var _verticalDB: Map[Int, Vector[Int]] = Map[Int, Vector[Int]]()
  val _accmu_N = sc.longAccumulator
  var _N: Int = 0
  var _M: Int = 0
  
  
  def printDB(): Unit = {
    println(" ---------- horizontal DB( No need to store )-------------")
    println(_horizontalDB.foreach(println))
    println(" ---------- vertical DB -------------")
    //    println(verticalDB.foreach(println))
    println(_verticalDB)
//    println("we have [" + allData.getNumPartitions + "] partitions")
  
    println("M = " + _M + ", N = " + _N)
  }
  
  def addTransaction(itemsString: Array[String]): (Set[Int], Vector[Int]) = {
    
    var itemset: Array[Int] = Array[Int]()
    var items: collection.mutable.Set[Int] = collection.mutable.Set[Int]()
    for(item <- itemsString) {
      items += item.toInt
      itemset = itemset :+ item.toInt
      
    }
    _accmu_N.add(1)
    //    horizontalDB = horizontalDB :+ itemset.toVector
    (items.toSet, itemset.toVector)
  }
  
  def loadFile(path: String) : Unit = {
    
    def map_convert2VerticalDB(x: (Int, (Set[Int], Vector[Int]))): (Set[Int], Map[Int, Vector[Int]]) = {
      
      var res = Map[Int, Vector[Int]]()
      for(item <- x._2._2) {
        if(res.contains(item)){
          val find = res.filter(y => y._1 == item).head
          res.updated(item, find._2 :+ x._1)
        } else {
          res = res + (item -> Vector[Int](x._1))
        }
      }
      
      (x._2._1, res)
    }
    
    def reduce_verticalDB(a: (Set[Int], Map[Int, Vector[Int]]), b: (Set[Int], Map[Int, Vector[Int]])) : (Set[Int], Map[Int, Vector[Int]]) = {
      def mergeTwoMap(map1: Map[Int, Vector[Int]], map2: Map[Int, Vector[Int]]): Map[Int, Vector[Int]] = {
        var res = map1
        for((key, vec) <- map2) {
          if(res.contains(key)) {
            res = res.updated(key, res(key) ++ map2(key))
          } else {
            res = res + (key -> vec)
          }
        }
        
        res
        
        
      }
      
      (a,b) match {
        case ((items1, map1), (items2, map2)) => (items1 ++ items2, mergeTwoMap(map1,map2))
      }
      
    }
    
    def reInitializeValue(verticalDB: (Set[Int], Map[Int, Vector[Int]])): Unit ={
      this._verticalDB = verticalDB._2
      this._items = verticalDB._1
      this._M = _items.count(x => true)
      this._N = this._accmu_N.value.toInt
    }
    
    
    
    val allData: RDD[String] = sc.textFile(path)
    
    //    allData.foreach(x => println)
    //    println(allData.toString())
    val horizontalDB: RDD[(Int, (Set[Int], Vector[Int]))] =
    allData.map(x => x.split(" ")) //RDD[Array[String]]
      .zipWithIndex()
      .map(x => (x._2.toInt + 1, addTransaction(x._1))) //RDD[ (Set[Int], Vector[Int]) ]
    
    val verticalDB: (Set[Int], Map[Int, Vector[Int]]) =
      horizontalDB.map(y =>map_convert2VerticalDB(y))
        .reduce((x, y) => reduce_verticalDB(x,y))
    
    
    reInitializeValue(verticalDB)
    
    
    printDB()
    
  }
}
