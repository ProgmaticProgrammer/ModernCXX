#include <iostream>
#include <algorithm>
#include "document.h"
#include "history.h"
using std::cout;
using std::reverse;

object_t func()
{
	object_t result = 5;
	return result;
}

int quiz_main()
{
	/*
	Quiz: What will this print? DEBUG - ctor,copy RELEASE - ctor RVO
	*/
	object_t x = 0;
	x = func();//use move= if available, or copy= if not
	return 0;
}

/* How easy for client to add a new class
* All you need it a func draw for the new class
* For all native type, supported by that template draw func.
*/
class my_class_t{};
void draw(const my_class_t& x, ostream& out, size_t position)
{
	out << string(position, ' ') << "my_class_t" << endl;
}

int doc_main()
{
	document_t document;
	document.reserve(5);

	document.emplace_back(0);
	document.emplace_back(1.5);
	document.emplace_back("Hello!!!");
	document.emplace_back(document);
	document.emplace_back(my_class_t{});

	// 2 copy here???
	reverse(document.begin(), document.end());

	draw(document, cout, 0);
	return 0;
}
int main()
{
	history_t h(1);
	current(h).emplace_back(0);
	current(h).emplace_back(string("Hello!"));
	draw(current(h), cout, 0);
	cout << "--------------------------" << endl;
	commit(h);
	current(h).emplace_back(current(h));
	current(h).emplace_back(my_class_t());
	current(h)[1] = string("World");
	draw(current(h), cout, 0);
	cout << "--------------------------" << endl;
	undo(h);
	draw(current(h), cout, 0);

	return 0;
}