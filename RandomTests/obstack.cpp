#include<vector>
#include<iostream>
using namespace std;
class IntStack{
public:
	vector<int> stack;
	int upper;
 int SIZE=16;
	int minimumsize =1<<(SIZE-1);
IntStack(){
	IntStack(SIZE);
}
IntStack(int SIZE){
	stack.reserve(SIZE);
	clear();
}
int getupper() {
	return upper;
}
int setupper(int upper) {
        return this->upper = upper;
}
void clear() {
        upper = -1;
}
bool isEmpty() {
        return upper < 0;
}
void push(int x){
	if(++upper >=stack.size())
		expand();
	stack[upper]=x;
}
int pop(){
	int r=stack[upper--];
	shrink();
	return r;
}
int get(int x){
	return stack[x];
}
void set(int x,int val){
	stack[x]=val;
}
int size(){
	return upper+1;
}
void expand(){
	int l=stack.size();
	stack.resize(l*4);
}
void shrink(){
	int l=stack.size();
	if(l<=minimumsize || upper<<2 >=1)
		return;
	l=1+(upper<< 1);
	if(upper <minimumsize)
		l=minimumsize;
	stack.resize(l);
}
vector<int> toArray(){
	vector<int> array;
	if(size() > 0)
		array=stack;
	return array;
}
void reverse(){

	int l=size();
	int h=l>>1;
	for(int x=0;x<h;x++){
		int temp=stack[x];
		stack[x]=stack[l-x-1];
		stack[l-x-1]=temp;
	}
}
};
int * randomsdq(int Seed){


        static int value[5];int random;

        for (int x=100,y=0;y<5;x=x*Seed,y++){
            random=x*Seed+Seed*(x-Seed);
            value[y]=random;
        }

        return value;

}

int main()
{
int* string1;int Seed;
cout<<"insert seed value ";
cin>>Seed;
string1=randomsdq(Seed);
IntStack s(string1[0]);
s.push(string1[0]);
s.push(string1[1]);
s.push(string1[2]);
s.push(string1[3]);
s.push(string1[4]);
cout<<string1[0]<<" is pushed"<<endl;
cout<<string1[1]<<" is pushed"<<endl;
cout<<string1[2]<<" is pushed"<<endl;
cout<<string1[3]<<" is pushed"<<endl;
cout<<string1[4]<<" is pushed"<<endl;
cout<<"Total stack size:";
cout<<s.getupper()+1<<endl;
}