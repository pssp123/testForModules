#include <iostream>
using namespace std;

struct student
{
  student(int age):_age(age){};
private:
  int _age;
public:
  bool operator==(const student st) const
  {
    if(this->_age==st._age)
      return true;
    else 
      return false;
  }
  
};
int main(int argc, char **argv) {
    student pp(22);
    student yanjing(22);
    if(pp==yanjing)
      cout<<"1"<<endl;
    return 0;
}
