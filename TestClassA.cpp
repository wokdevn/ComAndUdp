#include <iostream>
using namespace std;

class cb;
class ca {
public:
	int id;
	static cb* capr;
	ca(int i) {
		id = i;
	}
	void getid() {
		cout << id;
	}
};

class cb {
public:
	int id;
	static ca* cbpr;
	cb(int i) {
		id = i;
	}
	void getid() {
		cout << id;
	}
};

cb* ca::capr = NULL;
ca* cb::cbpr = NULL;

int main(){
	ca* aa = new ca(1);
	cb* bb = new cb(2);

	aa->capr = bb;
	bb->cbpr = aa;

	aa->capr->getid();
	bb->cbpr->getid();

	cin.get();
}