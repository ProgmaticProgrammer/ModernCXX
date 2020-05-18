#include <unity.h>

#include <iostream>

#include "matrix.h"

enum class Color { BLACK, RED, YELLOW, ORANGE };
constexpr size_t ROW = 3;
constexpr size_t COLUMN = 2;
using ColorMatrix = array<array<Color, COLUMN>, ROW>;
/////////////////////////
//    test cases       //
/////////////////////////

void test_MatrixAccess(void) {
  ColorMatrix matrix = {{{Color::BLACK, Color::RED},
                         {Color::YELLOW, Color::ORANGE},
                         {Color::RED, Color::BLACK}}};
  TEST_ASSERT_EQUAL(Color::BLACK, matrix[0][0]);
  TEST_ASSERT_EQUAL(Color::RED, matrix[0][1]);
  TEST_ASSERT_EQUAL(Color::YELLOW, matrix[1][0]);
  TEST_ASSERT_EQUAL(Color::ORANGE, matrix[1][1]);
  TEST_ASSERT_EQUAL(Color::RED, matrix[2][0]);
  TEST_ASSERT_EQUAL(Color::BLACK, matrix[2][1]);
}
void test_MatrixCreation(void) {
  const Matrix<Color, 2, 2> matrix = {
      {{{Color::BLACK, Color::RED}, {Color::YELLOW, Color::ORANGE}}}};
  TEST_ASSERT_EQUAL(Color::BLACK, matrix.at(0, 0));
  TEST_ASSERT_EQUAL(Color::RED, matrix.at(0, 1));
  TEST_ASSERT_EQUAL(Color::YELLOW, matrix.at(1, 0));
  TEST_ASSERT_EQUAL(Color::ORANGE, matrix.at(1, 1));
}
void test_MatrixVisit(void) {
  Matrix<Color, 2, 2> matrix = {
      {{{Color::BLACK, Color::RED}, {Color::YELLOW, Color::ORANGE}}}};
  for (auto it = matrix.begin(); it != matrix.end(); ++it) {
    switch (*it) {
      case Color::BLACK:
        std::cout << "BLACK" << std::endl;
        break;
      case Color::RED:
        std::cout << "RED" << std::endl;
        break;
      case Color::YELLOW:
        std::cout << "YELLOW" << std::endl;
        break;
      case Color::ORANGE:
        std::cout << "ORANGE" << std::endl;
        break;
      default:
        std::cout << "ORANGE" << std::endl;
        break;
    }
  }
}
/////////////////////////
//  Setup and register //
/////////////////////////
// call for each test
void setUp(void) {}
// call for each test
void tearDown(void) {}

int main(int argc, char** argv) {
  UNITY_BEGIN();
  // RUN_TEST(test_MatrixCreation);
  // RUN_TEST(test_MatrixAccess);
  RUN_TEST(test_MatrixVisit);
  return UNITY_END();
}
