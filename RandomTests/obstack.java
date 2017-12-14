import java.util.*;
import java.util.ArrayList;
class ObStack<T> extends ArrayList<T> {
  private static final long serialVersionUID = 1L;
  final T pop() {
    final int l = this.size() - 1;
    return this.remove(l);
  }
  final void push(final T O) {
    add(O);
  }
  final T peek() {
    return get(this.size() - 1);
  }
}
public class Obstackk {
	 public static int[] randomm()  {
	      int s=3;int[] d;int r;
	      d=new int[10];
	 	      for (int i=100,j=0;j<5;i=i*s,j++){
	          r=i*s+s*(i-s);
	          d[j]=r;
	      }
	      
	      return d;
	      }

	public static void main(String[] args) {
		ObStack<String> ob=new ObStack<String>();
		int[] a=randomm();
		String x=Integer.toString(a[0]);
		ob.push(x);
		System.out.println("after Pushing "+ x);
		String xx=Integer.toString(a[1]);
		ob.push(xx);
		System.out.println("after Pushing "+ xx);
		String xy=Integer.toString(a[2]);
		ob.push(xy);
		System.out.println("after Pushing "+ xy);
		String xxx=Integer.toString(a[3]);
		ob.push(xxx);
		System.out.println("after Pushing "+ xxx);
		System.out.println("Top after pushing "+ob.peek());
		ob.pop();
        System.out.println("top after the poping  "+ob.peek());
	}

}