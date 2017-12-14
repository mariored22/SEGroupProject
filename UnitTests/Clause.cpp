
#include "Clause.h"
#include <vector> 
#include <iostream> 

using namespace std;

	 
	
	 int main(){
		 int hgs[] = { 11,22,33 };
		 int xs[] = { 0, 1, 2 };
	 	Clause c(10, hgs, 3 ,0 , 3, xs, 3);
	 	cout << "length of heap slice: "<< c.len << endl;
	 	cout << "head goals pointing to cells in cs: ";
	     
		cout << "[";
		for (int i = 0; i < 3; i++) {
			cout << c.hgs[i] << ",";
		}
		cout << "]";

		
	 	cout << "\n heap where this starts: "<<  c.base<< endl;
	 	cout << "first after the end of the head: "<< c.neck<< endl;
	 	cout << "indexables in head: "<< endl;

		cout << "[";
		for (int i = 0; i < 3; i++) {
			cout << c.xs[i] << ",";
		}
		cout << "]\n";

		system("pause");
		  return 0;

	 }

	 /*
    //result
	length of heap slice: 10
	head goals pointing to cells in cs:  [11 22 33 ]
	heap where this starts: 0
	first after the end of the head: 3
	indexables in head:
	[0 1 2 ]
	*/
