//written by lissa
//date 2015 07 15
//
//pattern Subject and Observer.


//compile:g++ <this file name>


#include<iostream>
using namespace std;
#include<vector>
class Observer;
class ConcreteSubject;
class ConcreteObserver;

class Observer{
public:
	virtual void update(string,string)=0;
};

class Subject{
public:
	void attach(Observer* observer){
		m_vector.push_back(observer);
	};
	void detach(Observer*observer){
		vector<Observer*>::iterator it=m_vector.begin();
		for(;it!=m_vector.end();it++){
			if(*it==observer)
				m_vector.erase(it);
		}
	};
	void notify(string key,string value){
		vector<Observer*>::iterator it=m_vector.begin();
		for(;it!=m_vector.end();it++){
			(*it)->update(key,value);
		}
	};
protected:
	vector<Observer*> m_vector;
private:
};

class ConcreteSubject:public Subject{
public:
	void setName(string name){
		this->name=name;
		Subject::notify("name",this->name);
	};
private:
	string name;
};


class ConcreteObserver:public Observer{
public:
	virtual void update(string key,string value){
		cout<<"主题数据改变了"<<key<<":"<<value<<endl;
	};
};

int main(){
	Observer*ob1=new ConcreteObserver();
	Observer*ob2=new ConcreteObserver();
	Observer*ob3=new ConcreteObserver();

	ConcreteSubject *sub=new ConcreteSubject();

	sub->attach(ob1);
	sub->attach(ob2);
	sub->attach(ob3);
	
	sub->setName("lizhi");
	return 0;
}