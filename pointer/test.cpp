#include <iostream>
#include <stdio.h>
using namespace std;
int main()
{
int swap(int* m,int* n);
int a=10;
int b=5;
int* m=&a;
int* n=&b;
//printf("a=%p,b=%p\n",&a,&b);
printf("m=%d,m=%d\n",*m,*n);
swap(&a,&b);

return 0;
}

int swap(int* m,int* n)
{
int c;
c=*m;
*m=*n;
*n=c;
printf("m=%d,m=%d\n",*m,*n);
}
