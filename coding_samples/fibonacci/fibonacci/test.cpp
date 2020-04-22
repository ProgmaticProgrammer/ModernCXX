#include "pch.h"
#include <chrono>
#include "Fibonacci.h"

using namespace fibonacci_namespace;

TEST(PowerTest, ZeroTest) {
	EXPECT_EQ( 1, power(2, 0, std::multiplies<int>{}));
	EXPECT_EQ( 0, power(2, 0, std::plus<int>{}));
}

TEST(PowerTest, OneTest) {
	EXPECT_EQ( 2, power(2, 1, std::multiplies<int>{}));
	EXPECT_EQ( 2, power(2, 1, std::plus<int>{}));
}

TEST(PowerTest, MultipleTest) {
	EXPECT_EQ( 8, power(2, 3, std::multiplies<int>{}));
	EXPECT_EQ( 6, power(2, 3, std::plus<int>{}));

	EXPECT_EQ( 1024, power(2, 10, std::multiplies<int>{}));
	EXPECT_EQ( 20, power(2, 10, std::plus<int>{}));
}


TEST(FibonacciTest, StartingTest) {
	int result = fibonacci<int, size_t>(0);
	EXPECT_EQ(0, result);
	result = fibonacci<int, size_t>(1);
	EXPECT_EQ(1, result);
}

TEST(FibonacciTest, FollowingTest) {
	for (int n = 2; n != 1000; ++n) {
		int result_n = fibonacci<int, size_t>(n);
		int result_n_1 = fibonacci<int, size_t>(n - 1);
		int result_n_2 = fibonacci<int, size_t>(n - 2);
		EXPECT_EQ(result_n, result_n_1 + result_n_2);
	}
}

// for test purpose
static cpp_int fibonacci(size_t n) {
	assert(n >= 0);
	if (n == 0) return 0;
	if (n == 1) return 1;

	cpp_int result_n2 = 0;
	cpp_int result_n1 = 1;
	cpp_int result_n = result_n2 + result_n1;
	for (size_t i = 2; i != n; ++i)
	{
		result_n2 = result_n1;
		result_n1 = result_n;
		result_n = result_n1 + result_n2;
	}
	return result_n;
}

TEST(FibonacciTest, VeryLongDigitsTest) {

	for (size_t n = 3; n != 100; ++n) {
		auto result = fibonacci<cpp_int, size_t>(n);
		EXPECT_EQ(fibonacci(n), result);
	}
}

TEST(FibonacciTest, PerformanceTest) {
	auto start_time = std::chrono::steady_clock::now();

	cpp_int result = fibonacci<cpp_int, size_t>(1'000'000);//3s
	
	auto end_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_time - start_time;
	EXPECT_TRUE(elapsed_seconds.count() <= 5) << "The test took too long.";
}
