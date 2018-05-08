#include <iostream>
#include <math.h>
#include <typeinfo>
#include <eigen3/Eigen/Core>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <functional>
#include <deque>
#include <queue>
#include <list>
#include <forward_list>
#include <vector>
#include "assert.h"
#include "gtest/gtest.h"


using namespace std;
//dynamic_cast const_cast static_cast
/*//prepare
class Base{
public:
	virtual void run(){
		cout << "run" << endl;
	}
	int a = 1;
};
class Derived : public Base{
public:
	int b = 2;
};
//const_cast
void const_cast_invert() {
	int a = 1;
	const int b = a;
	int& c = const_cast<int&>(b);
	c = 2;
	cout << b << endl;
}

//static_cast
void static_cast_invert(){
	//example 1:
	int a = 1;
	double b = static_cast<double>(a);
	cout << b << endl;
	//example 2:
	int* p1 = &a;
	void* p2 = static_cast<void*>(p1);
}

//dynamic_cast{}
void dynamic_cast_invert(){
	//use dynamic then you can use derived class member
	{
		Base* p = new Derived();
		Derived *p1 = dynamic_cast<Derived*>(p);
		cout << "first: p1->b = " << p1->b << endl;
		delete(p);
	}
	//why use dynamic but not static
	{
		Base* p = new Base();
		Derived* p1 = dynamic_cast<Derived*>(p); //father to son,it's unsafe,and will return
		cout << "second:p1->b = " << p1->b << endl;
		Derived* p2 = static_cast<Derived*>(p);
		cout << "second: p2->b = " << p2->b << endl; //father to son,it's unsafe,but will not return;
		delete(p);
	}
	//use dynamic then you can limit using derived class member
	{
		Derived* p = new Derived();
		cout << "third:p->b" << p->b << endl;
		Base* p1 = dynamic_cast<Base*>(p);
		cout << "third:p1->a" << p1->a << endl;//it's will all safe;
		delete(p);
	}
}*/

//find_if
/*void find_if()
{
	vector<int> vec{36,5,3,4};
	int a = 5;
	auto loc = std::find_if(vec.begin(),vec.end(),[&](int it){
		return it==a;
	});
	auto dis = std::distance(vec.begin(),loc);
	cout << "the value can find:" << *loc << endl;
	cout << "dis:" << dis << endl;
}*/

//The destruct function of base class is virtual,when the pointer as base class
/*class A
{
public:
	A(){cout<<"1"<<endl;};
	virtual ~A(){cout<<"2"<<endl;};
};
class B : public A{
public:
	B() { cout<< "3"<<endl; }
	~B() override{ cout<< "4"<<endl; }
};*/

//explicit
/*class A{
public:
	explicit A(float a,float b){cout<<a<<endl;}
	//error when construct define as explicit.
	// Here,compiler will use "1" as default parameter to generate class A object temperature
	//A a = 1;
};*/

//std::function std::bind
/*int add(int a,int b){
	cout<< a + b << endl;
};
void function_bind(){
	vector<int> a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);

	//example 1
//	std::function<void(int)> fun = [](int it){cout<<it+1<<endl;};

	//example 2
//	std::function<void(int)> fun = bind(add,std::placeholders::_1,20);
//	for_each(a.begin(),a.end(),fun);

	//example 3
	std::function<void(int)> fun = bind([](int a, int b, int c){
		cout << a + a + b + c + c<< endl;
	},std::placeholders::_1,10,20);
	for_each(a.begin(),a.end(),fun);
}*/

//Lvalue Rvalue
/*void print_value(int& i)//Lvalue
{
	std::cout << "Lvalue:" << i << std::endl;
}
void print_value(int&& i)//Rvalue
{
	std::cout << "Rvalue:" << i << std::endl;
}
void Lvalue_Rvalue()
{
	int i = 10;
	print_value(i);
	print_value(std::move(i));
}*/

//singleton
/*class Singleton
{
public:
	static Singleton& Instance()
	{
		static Singleton instance;
		return instance;
	}
private:
	Singleton();
	~Singleton();
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);
};*/

//friend class && friend function
/*void friend_function(int num,A& a)
{
	a.age_A_ = num;
}
class A{
public:
	friend void friend_function(int num,A& a);
	friend class B;
	int sex_A_;

private:
	int age_A_;
};
class B{
public:
	int sex_B_;
	void friend_class(int num,A& a)
	{
		a.age_A_ = num;
	}
private:
	int age_B_;
};*/

//sort
/*void sort(){
	vector<int> a;
	a.push_back(3);
	a.push_back(2);
	a.push_back(1);
	std::sort(a.begin(),a.end(),[](int a,int b){
		return a < b;
	});
	std::sort(a.begin(),a.end(), greater<int>());
	cout<< a.at(0)<< endl;
	cout<< a.at(1)<< endl;
	cout<< a.at(2)<< endl;
}*/

//default delete
//A *p1 = new A();
/*class A{
public:
	A() = default;
//A() = delete;
	A(int a){
		age = a;
		cout<<age<<endl;
	};
	int age;
};*/

//std::remove_if
/*void remove_if(){
	vector<int> a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(2);
	a.push_back(2);
	a.push_back(2);
	a.push_back(3);
	a.erase(remove_if(a.begin(),a.end(),[](int n){
		if(n == 2)
			return true;
		else
			return false;
	}),a.end());
	for (auto &tmp: a) {
		cout<< tmp <<endl;
	}
}*/

//min_element max_element
/*void element(){
	vector<int> a;
	a.push_back(1);
	a.push_back(3);
	a.push_back(5);
	a.push_back(7);
	a.push_back(9);
	a.push_back(11);
	auto min = min_element(a.begin(),a.end());
	auto max = max_element(a.begin(),a.end());
	auto max_comp = max_element(a.begin(),a.end(),[](int n,int m){
		if((n + m) == 5)
			return true;
		else
			return false;
	});//if return false,then the first parameter is n.if return true,then the second parameter is n.
	cout << *min << endl;
	cout << *max << endl;
	cout << *max_comp<< endl;
}*/

//recursion
/*int compare(int a,int b){
	if(a < b)
		return -1;
	else if(a == b)
		return 0;
	else
		return 1;
}

int find(int a[], int val, int left, int right){
	int middle = (left + right) / 2;
	if(left <= right){
		switch(compare(val,a[middle])){
			case -1:
				cout << -1 << endl;
				return find(a,val,left,middle - 1);
			case 0:
				cout << 0 << endl;
				return middle;
			case 1:
				cout << 1 << endl;
				return find(a,val,middle + 1,right);
		}
	}
	return -1;
}
void recursion(){
	int val = 1;
	int a[]={1,2,3,4,5,6,7,8,9,10};
	auto index = find(a,val,0,9);
	cout<< index << endl;
}*/

//recursion fibonacci f(0)=f(1)=1 f(n)=f(n-1)+f(n-2) n>2
/*int cacul(int n){
	if(n == 0 || n == 1)
		return 1;
	else
		return cacul(n-1) + cacul(n - 2);
}
void recursion(){
	int n = 3;
	auto val = cacul(n);
	cout << val << endl;
}*/

/*class my_deque{
public:
	void push_back(int i){
		d.push_back(i);
		if(d.size() > 3)
			d.pop_front();
	}
	std::deque<int> d;
};*/

//std::unique
/*void unique_test(){
	vector<int> a;
	a.push_back(1);
	a.push_back(3);
	a.push_back(2);
	a.push_back(2);
	a.push_back(5);
	a.push_back(2);
	a.push_back(3);
	a.push_back(27);
	a.push_back(2);
	a.push_back(22);
	std::sort(a.begin(),a.end(),[&](int n,int m){
		if(n < m)
			return true;
		return false;
	});
	auto tmp = std::unique(a.begin(),a.end(),[&](int n,int m){
		if(n == m)
			return true;
		return false;
	});
	a.erase(tmp,a.end());
	for(const auto& b : a){
	}
}*/

//用TEST做简单测试
/*int fun(){
	return 1;
}

TEST(TestFactorial, OtherInput)
{
	EXPECT_EQ(2,fun());
	EXPECT_EQ(1,fun());
	EXPECT_EQ(3,fun());
//}

TEST(TestFactorial, ZEROInput)
{
	EXPECT_EQ(1,fun());
}*/

//Test fixture
/*class numberTest : public ::testing::Test{
protected:
	virtual void SetUp(){
		a = 1;
		b = 2;
		c = 3;
	}
	int a;
	int b;
	int c;
};

TEST_F(numberTest,a){
	EXPECT_EQ(1,a);
}
TEST_F(numberTest,b){
	EXPECT_EQ(2,b);
}
TEST_F(numberTest,c){
	EXPECT_EQ(2,c);
}*/

//GTest paramter
/*class numberTest : public ::testing::TestWithParam<int>{
};
TEST_P(numberTest,abc){
	int a = GetParam();
	EXPECT_EQ(1,a);
};
INSTANTIATE_TEST_CASE_P(1234,numberTest,testing::Values(1,2,3,4,5));

void runTest(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}*/

//copy function 拷贝函数 equal function 赋值函数
/*class A{
public:
	A(){};
//copy function 拷贝
	A(const A& a){
		this->age = a.age;
	}
//equal function
	A& operator=(const A& a){
		this->age = a.age;
		return *this;
	}
	int age{};
};*/

//std::function and function object
/*//normal funciton
int add(int a, int b){
	return a+b;
}
//function object
class Sub{
public:
	int operator()(const int& a, const int& b)
	{
		return a - b;
	}
};
void test(){
	std::list<std::function<int(int ,int)>> func_list;
	Sub sub;
	func_list.push_back(add);
	func_list.push_back(sub);
	func_list.push_back([](int a, int b){
		return a * b;
	});
	for(const auto& func : func_list){
		std::cout << func(4,2) << std::endl;
	}
}*/

int main(int argc, char* argv[]) {
	void (*fun)() = sort;
	fun();
	return 0;
}

