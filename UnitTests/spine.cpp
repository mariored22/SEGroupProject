// test-Spine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IntList.h"
#include "Spine.h"
//#include <vector>

#include<iostream>
 
using namespace std;
int main()
{
	Spine *s1 = new Spine(0, -1);
   
   int * gs0 = new int(3); gs0[0] = 11; gs0[1] = 22; gs0[2] = 33;
   vector<int> cs; 
   cs.insert(cs.end(), 0);// =  { 0, 1, 2 };
   cs.insert(cs.end(), 1);
   //cs.insert(cs.end(), 2);
   vector<int> gs ;
   gs.insert(gs.end(), 11);
   gs.insert(gs.end(), 22);
  // gs.insert(gs.end(), 33);

   Spine *s2 = new  Spine(gs0, 3, -2, gs, 5, 10, &cs);



   
   
   cout << "head: " << s1 ->hd << endl;
   cout << "base: " << s1 -> base << endl;
  
   cout << "[";
   for (int i = 0; i < 2; i++) {
	   cout << s2->gs[i] << ",";
   }
   cout << "]\n";
    

   cout <<"ttop: " << s2 -> ttop << endl;
   cout <<"s2 head : " << s2 -> hd<< endl;  
   cout <<"s2 k: "<< s2 ->k << endl; 
  // cout << "s2 cs:" <<s2->gs[i] << end1;    

   system("pause");
	 return 0;
}
