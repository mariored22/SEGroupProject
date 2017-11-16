import java.util.*;
import java.io.*;
public class palindromeTest {
    public static void main(String[]args)throws IOException {
    Scanner in = new Scanner(new FileReader("words.txt"));
    PrintWriter out = new PrintWriter(new FileWriter("palindromes.txt"));
    String []phrase = new String[100];
    String []palindromes = new String[50];
    String p;
    String s="END";
    while (phrase!=s){
    p =isPalindrome(phrase);
    }
    out.printf("\nThe palindromes are :%s\n ",p);
    in.close();
    out.close();
    }

public static boolean isPalindrome(String phrase) {

    int left  = 0;
    int right = phrase.length() -1;

 
    while (left < right) {

        if (phrase.charAt(left) != phrase.charAt(right)) {
            return false;
        }
        left++;
        right--;
    }

   return true;

}

}
