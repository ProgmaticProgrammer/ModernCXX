# CMake generated Testfile for 
# Source directory: C:/sw-testing/ModernCXX/coding_samples/fibonacci
# Build directory: C:/sw-testing/ModernCXX/coding_samples/fibonacci/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(fibonacci_test "C:/sw-testing/ModernCXX/coding_samples/fibonacci/build/fibonacci_test.exe")
set_tests_properties(fibonacci_test PROPERTIES  _BACKTRACE_TRIPLES "C:/sw-testing/ModernCXX/coding_samples/fibonacci/CMakeLists.txt;51;add_test;C:/sw-testing/ModernCXX/coding_samples/fibonacci/CMakeLists.txt;0;")
subdirs("googletest-build")
