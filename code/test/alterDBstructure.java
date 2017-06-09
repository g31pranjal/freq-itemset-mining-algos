package code.test;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URL;

import code.input.transaction_database_list_integers.TransactionDatabase;


public class alterDBstructure {

	public static void main(String args[]) throws IOException{
		
		String input = fileToPath("../../datasets/retail.txt");

		TransactionDatabase database = new TransactionDatabase();

		try {
			database.loadFile(input);
		} catch (IOException e) {
			e.printStackTrace();
		}

		System.out.println(database.getItemSize());

	}


	public static String fileToPath(String filename) throws UnsupportedEncodingException{
		URL url = alterDBstructure.class.getResource(filename);
		return java.net.URLDecoder.decode(url.getPath(),"UTF-8");
	}


}


