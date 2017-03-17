#include <iostream>
#include <string>
#include <stdio.h>
struct student
{
  student(int age,char name)
  {
    this->age=age;
    this->name=name; 
    
  }
private:
int age;
char name;

public:
  void print()
{
  printf("age=%d\n",age);
  printf("name=%c",name);
  
  
}
  
  
};

int main(int argc, char **argv) {
    char name='p';
    student pp(22,name);
    pp.print();
    return 0;
}
