#include <iostream>
using namespace std;
 struct student
 {
   student(int age,char name):_age(age),_name(name){}
 private:
   int _age;
   char _name;
  public:
    void print()
  {
    cout<<_age<<endl;
    cout<<_name<<endl;
    
  }
};
int main(int argc, char **argv) {
    student pp(22,'c');
    pp.print();
    return 0;
}
