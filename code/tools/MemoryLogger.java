package code.tools;

public class MemoryLogger {
	
	private static MemoryLogger instance = new MemoryLogger();

	private double maxMemory = 0;
	private double initialUsedMemory = 0;
	
	
	public static MemoryLogger getInstance(){
		return instance;
	}
	
	public double getMaxMemory() {
		return maxMemory;
	}

	public void reset(){
		Runtime.getRuntime().gc();
		maxMemory = 0;
		initialUsedMemory = (Runtime.getRuntime().totalMemory() -  Runtime.getRuntime().freeMemory()) / 1024d / 1024d;
		System.out.printf("initial usage : %.2f MB\n", initialUsedMemory);
	}
	
	public void checkMemory() {
		double currentMemory = (Runtime.getRuntime().totalMemory() -  Runtime.getRuntime().freeMemory()) / 1024d / 1024d;
		currentMemory -= initialUsedMemory;
		System.out.printf("current memory : %.2f MB\n", currentMemory);
		if (currentMemory > maxMemory) {
			maxMemory = currentMemory;
		}
		Runtime.getRuntime().gc();
	}
}
