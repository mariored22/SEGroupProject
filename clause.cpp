#include <algorithm>
#include <cstdlib>
#include <stack>
#include <cmath>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
/*
 ***************The clause is represnted in the following manner******************
 */
class Clause
{
public:
  int len;
  int * hgs;
  int base;
  int neck;
  int * xs;

  Clause( int len, int hgs[], int base, int neck,int xs[])
  {
   // head+goals pointing to cells in cs
    this->hgs=hgs;  
    // heap where this starts
    this->base=base; 
    // length of heap slice
    this->len=len; 
    // first after the end of the head
    this->neck=neck;
    // indexables in head
    this->xs= xs; 
    cout<<"The value of Length :";
    cout<<len;
    cout<<"\n";
    cout<<"The value of Base is: ";
    cout<<base;
    cout<<"\n";
    cout<<"The Index xs is : \n";
    for(int i=0;i<3;i++)
{
    cout << hgs[i];
    cout<<" ";

}
  cout<<"\n";
  cout<<"The Neck value is : ";
  cout<<neck;
   cout<<"\n";
    cout<<"The Heap value is : \n";
    for(int j=0;j<3;j++)
{
    cout << xs[j];
    cout<<" ";


}
}
};

int * randomfun(int seed){
        static int data[3];int random;

        for (int i=100,j=0;j<3;i=i*seed,j++){
            
            // defining random values 
            random=i*seed+seed*(i-seed);
            //assigning data value to random value 
            data[j]=random;
        }
        return data;
}

int main()

{
 
 int *str1;int *str2;
 int d1[4];
 int seed=35;
  //defining str1
  str1=randomfun(34);
  
  for (int h=0;h<3;h++)
  {
    
      d1[h]=str1[h];
  }
  
//defining str2  
str2=randomfun(35);
Clause b(seed+10,d1,seed+12,seed+35,str1);
return 0;

  
}