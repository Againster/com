//written by lissa
//date 2015 10 12
//
//pattern Decorator.


//compile:g++ <this file name>


#include <iostream>
using namespace std;

class basic_computer{
public:
	virtual int compute() = 0;
	virtual int display() = 0;
private:
};

class lenovo_computer: public basic_computer{
public:
	int compute(){
		cout<<"lenovo compute"<<endl;
		return 0;
	};
	int display(){
		cout<<"lenovo display"<<endl;
		return 0;
	};
private:
};

class sony_computer: public basic_computer{
public:
	int compute(){
		cout<<"sony compute"<<endl;
		return 0;
	};
	int display(){
		cout<<"sony display"<<endl;
		return 0;
	};
private:
};

class ibm_computer: public basic_computer{
public:
	int compute(){
		cout<<"ibm compute"<<endl;
		return 0;
	};
	int display(){
		cout<<"ibm display"<<endl;
		return 0;
	};
private:
};

class decorator_computer: public basic_computer{
public:
	explicit decorator_computer(basic_computer* x){
		this->x = x;
	};
	int compute(){
		x->compute();
		return 0;
	};
	int display(){
		x->display();
		return 0;
	};
private:
	basic_computer* x;
};

class decorator_computer_enforce: public decorator_computer{
public:
	explicit decorator_computer_enforce(basic_computer* x): \
		decorator_computer(x){
	};
	int artificial_intelligence(){
		cout<<"WOW, this computer is enforced to be an artificial intelligence computer."<<endl;
		return 0;
	};
};

int main(){
	lenovo_computer l;
	sony_computer s;
	ibm_computer i;
	decorator_computer_enforce d(&i);
	d.compute();
	d.display();
	d.artificial_intelligence();
	
	
	decorator_computer_enforce e(&s);
	e.compute();
	e.display();
	e.artificial_intelligence();
	
	
	decorator_computer_enforce f(&l);
	f.compute();
	f.display();
	f.artificial_intelligence();
    return 0;
}
