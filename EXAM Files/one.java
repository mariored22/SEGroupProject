import java.io.*;
class Check_Num{
public static void main(String args[]) throws IOException
   {
	int start=1,value;
	for(int i=1;i<=10;i++)
	{
		value=i;
			
		if(value%2==0)
		{
			value=value/2;
		}
		else
		{
			value= (2*value)+(value+1);
		}
	
	if(value==1)
	{
	System.out.println(i+"is reaching to 1");
        }
	else
	{
	System.out.println(i+"is reaching not to 1");
	}
	}
   }
}