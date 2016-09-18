import java.util.Scanner;

/**
 * Accepts input via stream redirection and prints input to console.
 *
 * @author Ted Mader
 * @since 2016-09-18
 */
public class InputTest {
  
  public static void main(String[] args) {
    Scanner input = new Scanner(System.in);
    while (input.hasNext()) {
      System.out.println(input.nextLine());
    }
  }
}
