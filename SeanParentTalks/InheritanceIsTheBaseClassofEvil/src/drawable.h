#ifndef DRAWABLE_HEADER
#define DRAWABLE_HEADER

#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include <string>
using std::string;
using std::shared_ptr;
using std::vector;
using std::string;
using std::ostream;
using std::cout;
using std::endl;
using std::move;
using std::make_shared;

template<typename T>
void draw(const T& x, ostream& out, size_t position)
{
	out << string(position, ' ') << x << endl;
}

// Drawable object which can hold any class implement the draw
// e.g. document_t in document.h and my_class_t in main.cxx
class object_t {
public:
	template<typename T>
	object_t(T x) : self_(make_shared<model<T>>(move(x))){ /*cout << "ctor" << endl;*/ }
	
	friend void draw(const object_t& x, ostream& out, size_t position)
	{
		x.self_->draw_(out, position);
	}
private:
	struct concept_t
	{
		virtual ~concept_t() = default;
		virtual void draw_(ostream& out, size_t position) const = 0;
	};

	template<typename T>
	struct model : concept_t {
		model(T x) : data_(move(x)) { }
		void draw_(ostream& out, size_t position) const override
		{
			draw(data_, out, position);
		}
		T data_;
	};
	shared_ptr<const concept_t> self_;//NOTE: const is a must here for value sematics
};

#endif