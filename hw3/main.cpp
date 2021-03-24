#include "String.h"
#include <iostream>
using namespace std;

int main(void)
{
    Str c(5);
	cout << c.contents()<<endl;
	cout << c.length()<<endl;
    c = "I'm a a";
	cout << c.contents()<<endl;
	cout << c.length()<<endl;
    Str a("I'm a girl");
	cout << a.contents()<<endl;
	cout << a.length()<<endl;
    a="I'm a boy\n";
	cout << a.contents()<<endl;
	cout << a.length()<<endl;
	cout << a.compare("I'm a a") << endl;
    cout << a.compare(c) << endl;
	return 0;
}
