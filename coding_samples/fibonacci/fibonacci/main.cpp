#include "pch.h"
#include <numeric>
#include <future>
#include <fstream>
#include <chrono>
#include "Fibonacci.h"

// calculate 1,000,000th Fibonacci number
// save it to file and calculation time as well
int __main() {
	auto start = std::chrono::steady_clock::now();
	//std::future<cpp_int> x = std::async([] { return fibonacci(1'000'000); });//call nontemplate one, 20s

	//std::future<cpp_int> x = std::async([] { return fibonacci<cpp_int>(1'000'000); });//return immediately
	//auto result = x.get();

	cpp_int result = fibonacci<cpp_int, size_t>(1'000'000);//3s
	
	auto end = std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::ofstream ofs{ "test.txt", std::ofstream::out };
	ofs << "elapsed time: " << elapsed_seconds.count() << "s\n";

	ofs << result << "n";
	return 0;
}