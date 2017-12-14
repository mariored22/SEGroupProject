class Clause {
  Clause(final int len, final int[] hgs, final int base, final int neck, final int[] xs) {
    // head+goals pointing to cells in cs
    this.hgs = hgs; 
    // heap where this starts
    this.base = base; 
    this.len = len; 
    // length of heap slice
    this.neck = neck; 
    // first after the end of the head
    this.xs = xs; 
    // indexables in head


    System.out.println("The Length value:"+len);
    System.out.println("The Base value:"+base);
    System.out.println("The Index value XS:");
    for(int i=0; i<xs.length;i++)
    System.out.print("  "+xs[i]);
    System.out.println();
    System.out.println("Value of the neck:"+neck);
    System.out.println("Heap: ");
    for(int i=0; i<hgs.length;i++)
        System.out.print("  "+hgs[i]);
  }
  final int len;
  final int[] hgs;
  final int base;
  final int neck;
  final int[] xs;

  }

public class ClauseTest{
	public static void main(String args[])
	{
	    int ab[]; ab=new int[3];int bc[]; bc=new int[3];
    //defining seed
        int seed=35;
	   
	    ab=random(34);
	
		bc=random(35);
		Clause cl=new Clause(seed+10,bc,seed+12,seed+35,ab);
	}
	
public static int[] random(int seed){
       int[] data;int random;
        data=new int[3];
         
         for (int i=100,j=0;j<3;i=i*seed,j++){
            //random value is defined as follows
            random=i*seed+seed*(i-seed);
            data[j]=random;

        }
        return data;

}

}