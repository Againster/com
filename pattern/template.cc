//written by lissa
//date 2017 04 11
//
//pattern Template.


//compile:g++ <this file name>


#include <iostream>
using namespace std;

class Game {
public:
  virtual void initialize() = 0;
  virtual void start() = 0;
  virtual void end() = 0;
  void play() {
    this->initialize();
    this->start();
    this->end();
  };
};

class SuperMario : public Game {
public:
  void initialize() {
    cout<<"super mario initializing..."<<endl;
  };

  void start() {
    cout<<"super mario starting..."<<endl;
  };

  void end() {
    cout<<"super mario end..."<<endl;
  };
};

class WarCraft: public Game {
public:
  void initialize() {
    cout<<"war craft initializing..."<<endl;
  };

  void start() {
    cout<<"war craft starting..."<<endl;
  };

  void end() {
    cout<<"war craft end..."<<endl;
  };
};

int main () {
  Game* g = new WarCraft();
  g->play();
  g = new SuperMario();
  g->play();
};

