
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.Scanner;

pubic class palindrom{

public static void main(String args[])
	   {
			int count =0;
			String original, reverse = "";
			List<String> lines = null;
		try {
			lines = Files.readAllLines(Paths.get("./wordlist.txt"), StandardCharsets.UTF_8);
		} catch (IOException e) {
			e.printStackTrace();
		}
	      
	      for(String original1: lines)
	      {
	    	  int length = original1.length();
	 
	    	  for ( int i = length - 1; i >= 0; i-- )
	    		  reverse = reverse + original1.charAt(i);
	    	  if (original1.equals(reverse)){
	    		  count++;
	    		  System.out.println("Entered string is a palindrome : "+original1);}
	      	}
	      
	      System.out.println("Total number of palindromes in the file: "+ count);
	   }
}