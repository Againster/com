//written by lissa
//date 2015 10 12
//
//pattern Singleton.


//compile:g++ <this file name>

#include <iostream>
using namespace std;

class singleton{
private:
    explicit singleton(){};
    ~singleton(){
        if(x!=NULL)
            delete x;
        x=NULL;
    };
    static singleton* x;
public:
    static singleton* getInstance(){
        if(x==NULL){
            x = new singleton;
        }
        return x;
    }
    int unique_var;
};
singleton* singleton::x = NULL;



int main(){
    singleton* x = singleton::getInstance();
    x->unique_var = 5;
    singleton* y = singleton::getInstance();
    cout<<y->unique_var<<endl;
    return 0;
}
