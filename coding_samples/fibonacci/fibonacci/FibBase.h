#pragma once
#include <array>
using std::array;

namespace fibonacci_namespace {

	///build the base of fibonacci matrix
	template <typename R, size_t n>
	struct FibBase
	{
		//enum { value = FibBase<R, n-1>::value * FibBase<R, n-1>::value};
		constexpr static const array<R, 4> value = { 
			FibBase<R, n - 1>::value[0] * FibBase<R, n - 1>::value[0] + FibBase<R, n - 1>::value[1] * FibBase<R, n - 1>::value[2],
			FibBase<R, n - 1>::value[0] * FibBase<R, n - 1>::value[1] + FibBase<R, n - 1>::value[1] * FibBase<R, n - 1>::value[3],
			FibBase<R, n - 1>::value[2] * FibBase<R, n - 1>::value[0] + FibBase<R, n - 1>::value[3] * FibBase<R, n - 1>::value[2],
			FibBase<R, n - 1>::value[2] * FibBase<R, n - 1>::value[1] + FibBase<R, n - 1>::value[3] * FibBase<R, n - 1>::value[3] };
	};

	template<typename R>
	struct FibBase<R, 1> {
		//enum { value = constexpr const array<R, 4>{ 1, 1, 1, 0 } };
		constexpr static const array<R, 4> value = array<R, 4>{ 1, 1, 1, 0 };
	};

	template<typename R>
	struct FibBase<R, 0> {
		//enum { value = constexpr const array<R, 4>{ 1, 0, 0, 1 } };
		constexpr static const array<R, 4> value = array<R, 4>{ 1, 0, 0, 1 };
	};

}
