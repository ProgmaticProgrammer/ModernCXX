#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <array>

using namespace boost::multiprecision;
using std::array;

/* How to abstract a template from concrete type
// pow(x,n) = x * x * ... * x
//int pow(int x, size_t n) {
//	int result = 1;
//
//	for (size_t i = 0; i != n; ++i)
//		result = result * x;
//
//	return result;
//}

//// sum(x,n) = x + x + ... + x
//int sum(int x, size_t n) {
//	int result = 0;
//
//	for (size_t i = 0; i != n; ++i)
//		result = result + x;
//
//	return result;
//}

//abstract base type(int above) as T, exponent type(size_t) as N
//operator type(+ or *) as O
template <typename T, typename N, typename O>
T power_intuitive_version(T x, N n, O op)
{
	T result = identity_element(op);

	for (N i = 0; i != n; ++i)
		result = op(result, x);

	return result;
}
*/

template <typename N>
N identity_element(const std::multiplies<N>&)
{
	return N{ 1 };
}
template <typename N>
N identity_element(const std::plus<N>&)
{
	return N{ 0 };
}

template <typename T, typename N, typename O>
T power(T x, N n, O op)
{
	// Mathematics to Generic Programming version
	if (n == 0) return identity_element(op);
	while ((n & 1) == 0) {
		n >>= 1;
		x = op(x, x);
	}
	T result = x;
	n >>= 1;
	while (n != 0) {
		x = op(x, x);
		if ((n & 1) != 0) result = op(result, x);
		n >>= 1;
	}
	return result;
}

/* Fibonacci number
https://en.wikipedia.org/wiki/Fibonacci_number
https://en.wikipedia.org/wiki/File:34*21-FibonacciBlocks.png
*/

//matrix way solve Fibonacci
//[1 1]n = [Fn+1 Fn  ]
//[1 0]    [Fn   Fn-1]
//use std::array<N, 4> represent the matrix
//need a struct op can do matrix_multiple

template <typename N>
struct multiply_2x2 {
	array<N, 4> operator()(const array<N, 4>& x, const array<N, 4>& y)
	{
		return { x[0] * y[0] + x[1] * y[2], x[0] * y[1] + x[1] * y[3],
		x[2] * y[0] + x[3] * y[2], x[2] * y[1] + x[3] * y[3] };
	}
};
template <typename N>
array<N, 4> identity_element(const multiply_2x2<N>&) { return { N(1), N(0), N(0), N(1) }; }

template <typename R, typename N>
R fibonacci(N n) {
	if (n == 0) return R(0);
	return power(array<R, 4>{ 1, 1, 1, 0 }, N(n - 1), multiply_2x2<R>())[0];
}
