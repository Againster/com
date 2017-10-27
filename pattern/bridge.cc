//written by lissa
//date 2017 04 11
//
//pattern Bridge.


//compile:g++ <this file name>


#include <iostream>
using namespace std;

// Implementator
class cycle {
public:
  virtual int draw_cycle() = 0;
};

// Implementator A
class draw_cycle_1 : public cycle {
public:
  int draw_cycle() {
    cout<<"draw cycle 1"<<endl;
    return 0;
  };
};

// Implementator B
class draw_cycle_2 : public cycle {
public:
  int draw_cycle() {
    cout<<"draw cycle 2"<<endl;
    return 0;
  };
};

// Abstraction
class shap {
public:
  virtual int draw_cycle()=0;
  virtual int draw_rectangle()=0;
};

// Redifined Abstraction
class draw_shap : public shap {
public:

  cycle* d;

  draw_shap(cycle* c) {
    d = c;
  };

  int draw_cycle() {
    d->draw_cycle();
    return 0;
  };

  int draw_rectangle() {
    cout<<"draw rectangle"<<endl;
    return 0;
  };
};

int main () {
  draw_shap d(new draw_cycle_2());
  draw_shap d2(new draw_cycle_1());
  d.draw_cycle();
  d2.draw_cycle();
  d.draw_rectangle();
  return 0;
}

