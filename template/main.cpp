#include <iostream>
using namespace std;

struct student
{

template <typename T>
T sum(T english,T math)
{
T total;
total=english+math;
return total;
}

};


int main()
{

student pp;
double english=10;
double math=20;
double total=pp.sum(english,math);
cout<<total<<endl;

return 0;
}