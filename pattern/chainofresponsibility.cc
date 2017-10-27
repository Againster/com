#include <iostream>
#include <list>
using namespace std;

class chain_of_responsibility {
public:
	void attach(chain_of_responsibility* responsibility) {
		chain.push_back(responsibility);
	};

	void detach(chain_of_responsibility* responsibility) {
		list<chain_of_responsibility*>::iterator it;
		for (it = chain.begin(); it != chain.end(); it++) {
			if (*it == responsibility) {
				chain.erase(it);
				break;
			}
		}
	}

	virtual void* handle(string key, void* value) = 0;
protected:
	list<chain_of_responsibility*> chain;
};

/*
class book : public chain_of_responsibility{
	virtual void* handle(string key, void* value) {
		cout<< "book" <<endl;
		chain.front()->handle(key, value);
		return NULL;
	}
};

class product: public chain_of_responsibility {
	virtual void* handle(string key, void* value) {
		cout<< "product" <<endl;
	  chain.front()->handle(key, value);
		return NULL;
	}
};

class retail : public chain_of_responsibility {
	virtual void* handle(string key, void* value) {
		cout<< "retail" <<endl;
		return NULL;
	}
};

int main () {
	chain_of_responsibility* rb = new book();
	chain_of_responsibility* rp = new product();
	chain_of_responsibility* rr = new retail();
	rb->attach(rp);
	rp->attach(rr);
	rb->handle("yes", (void*)"haha");
	return 0;
}
*/
