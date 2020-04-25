#ifndef HISTORY_HEADER
#define HISTORY_HEADER

#include <vector>
#include "document.h"
#include <cassert>
using std::vector;


using history_t = vector<document_t>;

void commit(history_t& x) { assert(x.size()); x.push_back(x.back()); }
void undo(history_t& x) { assert(x.size()); x.pop_back(); }
document_t& current(history_t& x) { assert(x.size()); return x.back(); }

#endif