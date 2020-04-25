#ifndef DOCUMENT_HEADER
#define DOCUMENT_HEADER

#include <ostream>
#include <vector>
#include "drawable.h"
using std::vector;
using std::cout;

using document_t = vector<object_t>;
void draw(const document_t& x, ostream& out, size_t position)
{
	out << string(position, ' ') << "<document>" << endl;
	for (const auto& e : x) draw(e, out, position + 2);
	out << string(position, ' ') << "</document>" << endl;
}

#endif